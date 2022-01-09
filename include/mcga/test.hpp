#pragma once

#include <concepts>
#include <cstdlib>
#include <cstring>
#include <utility>

namespace mcga::test {

namespace internal {

template<class T>
concept executable_t = requires(const T& obj) {
    { obj() } -> std::same_as<void>;
};

template<class T>
concept std_string_like = requires(const T& str) {
    { str.data() } -> std::same_as<const char*>;
};

}  // namespace internal

class String {
    bool isOwned;
    const char* data;

    static const char* duplicate_str(const char* data) {
        const auto len = std::strlen(data);
        const auto dup = static_cast<char*>(std::malloc(len + 1));
        std::strcpy(dup, data);
        return dup;
    }

  public:
    String() noexcept: String("") {
    }

    String(const char* data) noexcept: isOwned(false), data(data) {
    }

    template<int N>
    String(char (&data)[N]) noexcept: isOwned(false), data(data) {
    }

    template<int N>
    String(const char (&data)[N]) noexcept: isOwned(false), data(data) {
    }

    template<internal::std_string_like S>
    String(const S& data): isOwned(true), data(duplicate_str(data.data())) {
    }

    String(const String& other)
            : isOwned(other.isOwned),
              data(isOwned ? duplicate_str(other.data) : other.data) {
    }

    String(String&& other) noexcept: isOwned(other.isOwned), data(other.data) {
        other.isOwned = false;
    }

    String& operator=(const String& other) {
        if (this != &other) {
            this->~String();
            isOwned = other.isOwned;
            data = isOwned ? duplicate_str(other.data) : other.data;
        }
        return *this;
    }

    String& operator=(String&& other) noexcept {
        if (this != &other) {
            this->~String();
            isOwned = other.isOwned;
            data = other.data;
            other.isOwned = false;
        }
        return *this;
    }

    ~String() {
        if (isOwned) {
            std::free((void*)data);
        }
    }

    [[nodiscard]] const char* c_str() const noexcept {
        return data;
    }
};

struct Context {
    String verb{"Failed"};
    String fileName;
    String functionName;
    int line;
    int column;

    explicit Context(const char* fileName = __builtin_FILE(),
                     const char* functionName = __builtin_FUNCTION(),
                     int line = __builtin_LINE(),
                     int column = __builtin_COLUMN())
            : fileName(fileName), functionName(functionName), line(line),
              column(column) {
    }
};

struct Executable {
    void (*body)(void*);
    void (*dtor)(void*);
    void* data;
    Context context;

    template<internal::executable_t Callable>
    explicit Executable(Callable callable, Context context)
            : body([](void* d) {
                  (*static_cast<Callable*>(d))();
              }),
              dtor([](void* d) {
                  delete static_cast<Callable*>(d);
              }),
              data(new Callable(std::move(callable))),
              context(std::move(context)) {
    }

    Executable(const Executable&) = delete;

    Executable& operator=(const Executable&) = delete;

    Executable(Executable&& other) noexcept
            : body(other.body), dtor(other.dtor), data(other.data),
              context(std::move(other.context)) {
        other.data = nullptr;
    }

    Executable& operator=(Executable&& other) noexcept {
        if (this != &other) {
            dtor(data);
            body = other.body;
            dtor = other.dtor;
            data = other.data;
            context = std::move(other.context);
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

void register_failure(String message, Context context);

void register_cleanup(Executable exec);

template<bool isOptional>
struct MultiRunTestApi {
    template<executable_t Callable>
    void operator()(int numRuns,
                    TestConfig config,
                    Callable body,
                    Context context = Context()) const {
        config.attempts = numRuns;
        config.requiredPassedAttempts = numRuns;
        config.optional = isOptional;
        register_test(std::move(config), Executable(body, std::move(context)));
    }

    template<executable_t Callable>
    void operator()(int numRuns,
                    String description,
                    Callable body,
                    Context context = Context()) const {
        (*this)(numRuns,
                TestConfig{.description = std::move(description)},
                std::move(body),
                std::move(context));
    }

    template<executable_t Callable>
    void operator()(int numRuns,
                    Callable body,
                    Context context = Context()) const {
        (*this)(numRuns, TestConfig{}, std::move(body), std::move(context));
    }
};

template<bool isOptional>
struct RetryTestApi {
    template<executable_t Callable>
    void operator()(int attempts,
                    TestConfig config,
                    Callable body,
                    Context context = Context()) const {
        config.attempts = attempts;
        config.requiredPassedAttempts = 1;
        config.optional = isOptional;
        register_test(std::move(config),
                      Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(int attempts,
                    String description,
                    Callable body,
                    Context context = Context()) const {
        register_test(TestConfig{.description = std::move(description),
                                 .optional = isOptional,
                                 .attempts = attempts,
                                 .requiredPassedAttempts = 1},
                      Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(int attempts,
                    Callable body,
                    Context context = Context()) const {
        register_test(TestConfig{.optional = isOptional,
                                 .attempts = attempts,
                                 .requiredPassedAttempts = 1},
                      Executable(std::move(body), std::move(context)));
    }
};

template<bool isOptional>
struct OptionalTestApi {
    template<executable_t Callable>
    void operator()(TestConfig config,
                    Callable body,
                    Context context = Context()) const {
        config.optional = isOptional;
        register_test(std::move(config),
                      Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(String description,
                    Callable body,
                    Context context = Context()) const {
        register_test(
          TestConfig{
            .description = std::move(description),
            .optional = isOptional,
          },
          Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(Callable body, Context context = Context()) const {
        register_test(TestConfig{.optional = isOptional},
                      Executable(std::move(body), std::move(context)));
    }

    [[no_unique_address]] MultiRunTestApi<isOptional> multiRun;
    [[no_unique_address]] RetryTestApi<isOptional> retry;
};

struct TestApi : public OptionalTestApi<false> {
    [[no_unique_address]] OptionalTestApi<true> optional;
};

template<bool isOptional>
struct OptionalGroupApi {
    template<executable_t Callable>
    void operator()(GroupConfig config,
                    Callable body,
                    Context context = Context()) const {
        config.optional = isOptional;
        register_group(std::move(config),
                       Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(String description,
                    Callable body,
                    Context context = Context()) const {
        (*this)(GroupConfig{.description = std::move(description)},
                std::move(body),
                std::move(context));
    }

    template<executable_t Callable>
    void operator()(Callable body, Context context = Context()) const {
        (*this)(GroupConfig(), std::move(body), std::move(context));
    }
};

struct GroupApi : public OptionalGroupApi<false> {
    [[no_unique_address]] OptionalGroupApi<true> optional;
};

}  // namespace internal

struct TestCase {
    TestCase* next = nullptr;

    String name;
    void (*body)() = nullptr;
    Context context;

    explicit TestCase(String name, Context context = Context()) noexcept
            : name(std::move(name)), body(nullptr),
              context(std::move(context)) {
    }

    explicit TestCase(Context context = Context()) noexcept
            : name(""), body(nullptr), context(std::move(context)) {
    }

    TestCase(TestCase&& other) noexcept
            : name(std::move(other.name)), body(other.body),
              context(std::move(other.context)) {
        internal::register_test_case(this);
    }
    TestCase(const TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;
    TestCase& operator=(TestCase&&) = delete;

    TestCase&& operator+(void (*body_)()) && noexcept {
        body = body_;
        return std::move(*this);
    }
};

inline constexpr internal::TestApi test;
inline constexpr internal::GroupApi group;

template<internal::executable_t Callable>
void setUp(Callable func, Context context = Context()) {
    internal::register_set_up(Executable(std::move(func), std::move(context)));
}

template<internal::executable_t Callable>
void cleanup(Callable func, Context context = Context()) {
    internal::register_cleanup(Executable(std::move(func), std::move(context)));
}

template<internal::executable_t Callable>
void tearDown(Callable func, Context context = Context()) {
    internal::register_tear_down(
      Executable(std::move(func), std::move(context)));
}

inline void fail(String message = String(), Context context = Context()) {
    internal::register_failure(std::move(message), std::move(context));
}

inline void expect(bool expr, Context context = Context()) {
    if (!expr) {
        context.verb = "Expectation failed";
        fail("", std::move(context));
    }
}

}  // namespace mcga::test
