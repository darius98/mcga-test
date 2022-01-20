#pragma once

namespace mcga::test {

namespace internal {

template<class T, class U>
inline constexpr bool same_as_impl = false;

template<class T>
inline constexpr bool same_as_impl<T, T> = true;

template<class T, class U>
concept same_as = same_as_impl<T, U>;

template<class T>
concept executable_t = requires(const T& obj) {
    { obj() } -> same_as<void>;
};

template<class T>
concept std_string_like = requires(const T& str) {
    { str.data() } -> same_as<const char*>;
};

const char* duplicate_str(const char* data);

void delete_str(const char* data);

template<class T>
constexpr T&& move(T&& t) noexcept {
    return static_cast<T&&>(t);
}

template<class T>
constexpr T&& move(T& t) noexcept {
    return static_cast<T&&>(t);
}

}  // namespace internal

class String {
    bool isOwned;
    const char* data;

  public:
    constexpr String() noexcept: String("") {
    }

    constexpr String(const char* data) noexcept: isOwned(false), data(data) {
    }

    template<int N>
    constexpr String(const char (&data)[N]) noexcept
            : isOwned(false), data(data) {
    }

    String(const internal::std_string_like auto& data)
            : isOwned(true), data(internal::duplicate_str(data.data())) {
    }

    constexpr String(const String& other)
            : isOwned(other.isOwned),
              data(isOwned ? internal::duplicate_str(other.data) : other.data) {
    }

    constexpr String(String&& other) noexcept
            : isOwned(other.isOwned), data(other.data) {
        other.isOwned = false;
    }

    constexpr String& operator=(const String& other) {
        if (this != &other) {
            this->~String();
            isOwned = other.isOwned;
            data = isOwned ? internal::duplicate_str(other.data) : other.data;
        }
        return *this;
    }

    constexpr String& operator=(String&& other) noexcept {
        if (this != &other) {
            this->~String();
            isOwned = other.isOwned;
            data = other.data;
            other.isOwned = false;
        }
        return *this;
    }

    constexpr ~String() {
        if (isOwned) {
            internal::delete_str(data);
        }
    }

    [[nodiscard]] constexpr const char* c_str() const noexcept {
        return data;
    }
};

struct Context {
    const char* verb;
    const char* fileName;
    const char* functionName;
    int line;
    int column;

    explicit Context(const char* verb = "Failed",
                     const char* fileName = __builtin_FILE(),
                     const char* functionName = __builtin_FUNCTION(),
                     int line = __builtin_LINE(),
                     int column = __builtin_COLUMN())
            : verb(verb), fileName(fileName), functionName(functionName),
              line(line), column(column) {
    }
};

struct Executable {
    void (*body)(void*);
    void (*dtor)(void*);
    void* data;
    Context context;

    template<internal::executable_t Callable>
    Executable(Callable callable, const Context& context)
            : body([](void* d) {
                  (*static_cast<Callable*>(d))();
              }),
              dtor([](void* d) {
                  delete static_cast<Callable*>(d);
              }),
              data(new Callable(internal::move(callable))), context(context) {
    }

    Executable(const Executable&) = delete;

    Executable& operator=(const Executable&) = delete;

    Executable(Executable&& other) noexcept
            : body(other.body), dtor(other.dtor), data(other.data),
              context(other.context) {
        other.data = nullptr;
    }

    Executable& operator=(Executable&& other) noexcept {
        if (this != &other) {
            dtor(data);
            body = other.body;
            dtor = other.dtor;
            data = other.data;
            context = other.context;
            other.data = nullptr;
        }
        return *this;
    }

    ~Executable() {
        dtor(data);
    }

    void operator()() const {
        body(data);
    }
};

/** Structure defining the configuration for a test.
 *
 * Pass an instance of this class to the test() function. */
struct TestConfig {
    /** Description of the test.
     *
     * A test should provide a concise, yet clear and explicit description, both
     * for future maintainers of the test and UIs of this library. */
    String description;

    /** Whether this test is optional.
     *
     * If only tests that have this flag marked as `true` fail during a test
     * run, the test run will still exit with code `0`, meaning a successful
     * finish. UI can provide extra feedback based on this flag, like marking a
     * failed optional test in yellow instead of red (for example). */
    bool optional = false;

    /** The time limit (in a canonical unit of `ticks`) of this test.
     *
     * The time `tick` is a unit introduced to make tests more _portable_. If
     * the time limits and measurements were in, say, seconds, running the same
     * test on different hardware might have different results, making the test
     * _flaky_. The time tick is calculated to be approximately one second on a
     * 3.0GHz processor, yet this may vary based on hardware. The library
     * provides an option of displaying the time tick to absolute time ratio
     * through the getTimeTickLengthMs() function. User interfaces of this
     * library should provide the time of execution of a test both in ticks and
     * in a standard unit of time. */
    double timeTicksLimit = 1.0;

    /** The number of times this test should be attempted.
     *
     * This item should be configured for non-deterministic tests, together with
     * #requiredPassedAttempts. */
    int attempts = 1;

    /** The number of attempts that must be passed for the test to be considered
     * passed overall.
     *
     * This should be at most equal to #attempts. */
    int requiredPassedAttempts = 1;
};

/** Structure defining the configuration for a group.
 *
 * Pass an instance of this class to the group() function. */
struct GroupConfig {
    /** Description of the group.
     *
     * A group should provide a concise, yet clear and explicit description,
     * both for future maintainers of the group and user interfaces of this
     * library. */
    String description;

    /** Flag that marks this whole group as optional.
     *
     * This is equivalent to marking each individual test inside the group and
     * all its subgroups as optional. */
    bool optional = false;
};

struct TestCase;

namespace internal {

void register_test_case(TestCase* testCase) noexcept;

void register_test(TestConfig config, Executable body);

void register_group(GroupConfig config, Executable body);

void register_set_up(Executable body);

void register_tear_down(Executable body);

void register_interrupt(bool isFail, String message, Context context);

void register_cleanup(Executable exec);

}  // namespace internal

struct TestCase {
    TestCase* next = nullptr;

    const char* name;
    void (*body)() = nullptr;
    Context context;

    explicit TestCase(const char* name, Context context = Context()) noexcept
            : name(name), body(nullptr), context(context) {
    }

    explicit TestCase(Context context = Context()) noexcept
            : name(""), body(nullptr), context(context) {
    }

    TestCase(TestCase&& other) noexcept
            : name(other.name), body(other.body), context(other.context) {
        internal::register_test_case(this);
    }
    TestCase(const TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;
    TestCase& operator=(TestCase&&) = delete;

    TestCase&& operator+(void (*body_)()) && noexcept {
        body = body_;
        return static_cast<TestCase&&>(*this);
    }
};

void group(internal::executable_t auto body, Context context = Context()) {
    internal::register_group({}, Executable(internal::move(body), context));
}

void group(String description,
           internal::executable_t auto body,
           Context context = Context()) {
    internal::register_group({.description = internal::move(description)},
                             Executable(internal::move(body), context));
}

void group(GroupConfig config,
           internal::executable_t auto body,
           Context context = Context()) {
    internal::register_group(internal::move(config),
                             Executable(internal::move(body), context));
}

void test(internal::executable_t auto body, Context context = Context()) {
    internal::register_test({}, Executable(internal::move(body), context));
}

void test(String description,
          internal::executable_t auto body,
          Context context = Context()) {
    internal::register_test({.description = internal::move(description)},
                            Executable(internal::move(body), context));
}

void test(TestConfig config,
          internal::executable_t auto body,
          Context context = Context()) {
    internal::register_test(internal::move(config),
                            Executable(internal::move(body), context));
}

void setUp(internal::executable_t auto func, Context context = Context()) {
    internal::register_set_up(Executable(internal::move(func), context));
}

void cleanup(internal::executable_t auto func, Context context = Context()) {
    internal::register_cleanup(Executable(internal::move(func), context));
}

void tearDown(internal::executable_t auto func, Context context = Context()) {
    internal::register_tear_down(Executable(internal::move(func), context));
}

inline void fail(String message = String(), Context context = Context()) {
    internal::register_interrupt(true, internal::move(message), context);
}

inline void skip(String message = String(),
                 Context context = Context("Skipped")) {
    internal::register_interrupt(false, internal::move(message), context);
}

inline void expect(bool expr, Context context = Context("Expectation failed")) {
    if (!expr) {
        fail("", context);
    }
}

}  // namespace mcga::test
