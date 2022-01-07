#pragma once

#include <string>
#include <type_traits>

#include "mcga/internal/source_location.hpp"

namespace mcga::test {

namespace internal {

template<class T, class R, class... Args>
concept invocable_with = requires {
    std::is_invocable_r_v<R, T, Args...>;
};

template<class T>
concept executable_t = invocable_with<T, void>;

}  // namespace internal

struct Context {
    std::string verb = "Failed";
    std::string fileName;
    std::string functionName;
    uint32_t line;
    uint32_t column;

    explicit Context(internal::source_location location
                     = internal::source_location::current())
            : fileName(location.file_name()),
              functionName(location.function_name()), line(location.line()),
              column(location.column()) {
    }
};

struct Executable {
    void (*body)(void*);
    void (*dtor)(void*);
    void* data;
    Context context;

    template<internal::executable_t Callable>
    explicit Executable(Callable callable, Context context)
            : body([](void* d) { (*static_cast<Callable*>(d))(); }),
              dtor([](void* d) { delete static_cast<Callable*>(d); }),
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
    std::string description;

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
    std::size_t attempts = 1;

    /** The number of attempts that must be passed for the test to be considered
     * passed overall.
     *
     * This should be at most equal to #attempts. */
    std::size_t requiredPassedAttempts = 1;
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
    std::string description;

    /** Flag that marks this whole group as optional.
     *
     * This is equivalent to marking each individual test inside the group and
     * all its subgroups as optional. */
    bool optional = false;
};

namespace internal {

struct TestCase;

extern "C" void mcga_test_register_test_case(TestCase* testCase);

extern "C" void mcga_test_register_test(TestConfig config, Executable body);

extern "C" void mcga_test_register_group(GroupConfig config, Executable body);

extern "C" void mcga_test_register_set_up(Executable body);

extern "C" void mcga_test_register_tear_down(Executable body);

extern "C" void mcga_test_register_failure(std::string message,
                                           Context context);

template<bool isOptional>
struct MultiRunTestApi {
    template<executable_t Callable>
    void operator()(std::size_t numRuns,
                    TestConfig config,
                    Callable body,
                    Context context = Context()) const {
        config.attempts = numRuns;
        config.requiredPassedAttempts = numRuns;
        config.optional = isOptional;
        mcga_test_register_test(std::move(config),
                                Executable(body, std::move(context)));
    }

    template<executable_t Callable>
    void operator()(std::size_t numRuns,
                    std::string description,
                    Callable body,
                    Context context = Context()) const {
        (*this)(numRuns,
                TestConfig{.description = std::move(description)},
                std::move(body),
                std::move(context));
    }

    template<executable_t Callable>
    void operator()(std::size_t numRuns,
                    Callable body,
                    Context context = Context()) const {
        (*this)(numRuns, TestConfig{}, std::move(body), std::move(context));
    }
};

template<bool isOptional>
struct RetryTestApi {
    template<executable_t Callable>
    void operator()(std::size_t attempts,
                    TestConfig config,
                    Callable body,
                    Context context = Context()) const {
        config.attempts = attempts;
        config.requiredPassedAttempts = 1;
        config.optional = isOptional;
        mcga_test_register_test(
          std::move(config), Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(std::size_t attempts,
                    std::string description,
                    Callable body,
                    Context context = Context()) const {
        mcga_test_register_test(
          TestConfig{.description = std::move(description),
                     .optional = isOptional,
                     .attempts = attempts,
                     .requiredPassedAttempts = 1},
          Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(std::size_t attempts,
                    Callable body,
                    Context context = Context()) const {
        mcga_test_register_test(
          TestConfig{.optional = isOptional,
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
        mcga_test_register_test(
          std::move(config), Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(std::string description,
                    Callable body,
                    Context context = Context()) const {
        mcga_test_register_test(
          TestConfig{
            .description = std::move(description),
            .optional = isOptional,
          },
          Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(Callable body, Context context = Context()) const {
        mcga_test_register_test(
          TestConfig{.optional = isOptional},
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
        mcga_test_register_group(
          std::move(config), Executable(std::move(body), std::move(context)));
    }

    template<executable_t Callable>
    void operator()(std::string description,
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

struct SetUpApi {
    template<executable_t Callable>
    void operator()(Callable func, Context context = Context()) const {
        mcga_test_register_set_up(
          Executable(std::move(func), std::move(context)));
    }
};

struct TearDownApi {
    template<executable_t Callable>
    void operator()(Callable func, Context context = Context()) const {
        mcga_test_register_tear_down(
          Executable(std::move(func), std::move(context)));
    }
};

struct TestCase {
    TestCase* next;

    void (*body)();

    const char* name;
    internal::source_location location;

    TestCase(void (*body)(),
             const char* name,
             internal::source_location location) noexcept
            : body(body), name(name), location(location), next(nullptr) {
        mcga_test_register_test_case(this);
    }
};

}  // namespace internal

inline constexpr internal::TestApi test;
inline constexpr internal::GroupApi group;
inline constexpr internal::SetUpApi setUp;
inline constexpr internal::TearDownApi tearDown;

inline void fail(std::string message = std::string(),
                 Context context = Context()) {
    internal::mcga_test_register_failure(std::move(message),
                                         std::move(context));
}

inline void expect(bool expr, Context context = Context()) {
    if (!expr) {
        context.verb = "Expectation failed";
        fail("", std::move(context));
    }
}

}  // namespace mcga::test

#define INTERNAL_TEST_CASE_CAT(a, b) a##b
#define INTERNAL_TEST_CASE_CAT2(a, b) INTERNAL_TEST_CASE_CAT(a, b)
#define INTERNAL_TEST_CASE_UNIQ(a) INTERNAL_TEST_CASE_CAT2(a, __LINE__)

#define TEST_CASE(description)                                                 \
    namespace {                                                                \
    struct INTERNAL_TEST_CASE_UNIQ(TestCaseCls) {                              \
        static void testCase();                                                \
    };                                                                         \
    ::mcga::test::internal::TestCase INTERNAL_TEST_CASE_UNIQ(TestCaseReg)(     \
      INTERNAL_TEST_CASE_UNIQ(TestCaseCls)::testCase,                          \
      "" description,                                                          \
      ::mcga::test::internal::source_location::current());                     \
    }                                                                          \
    void INTERNAL_TEST_CASE_UNIQ(TestCaseCls)::testCase()
