#pragma once

#include <mcga/test/config.hpp>
#include <mcga/test/executable.hpp>
#include <mcga/test/string.hpp>

namespace mcga::test {
inline namespace MCGA_TEST_INTERNAL_ABI_NS {

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
     * This should be at most equal to #attempts, or -1 to automatically take
     * the same value as #attempts. */
    int requiredPassedAttempts = -1;
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

void group(GroupConfig config, Executable body);
inline void group(Executable body) {
    group(GroupConfig{}, std::move(body));
}
inline void group(String description, Executable body) {
    group(GroupConfig{.description = std::move(description)}, std::move(body));
}

void test(TestConfig config, Executable body);
inline void test(Executable body) {
    test(TestConfig{}, std::move(body));
}
inline void test(String description, Executable body) {
    test(TestConfig{.description = std::move(description)}, std::move(body));
}

void setUp(Executable body);
void tearDown(Executable body);
void cleanup(Executable body);

void fail(String message = String(), Context context = Context());
inline void expect(bool expr, Context context = Context("Expectation failed")) {
    if (!expr) {
        fail("", context);
    }
}
void skip(String message = String(), Context context = Context("Skipped"));

}  // namespace MCGA_TEST_INTERNAL_ABI_NS
}  // namespace mcga::test

namespace mcga::test::internal {

int register_test_case(const char* name,
                       void (*body)(),
                       Context context = Context());

struct empty_fixture {};

template<class F>
void fixtureTestCaseBody() {
    alignas(F) unsigned char f_storage[sizeof(F)];
    F* instance = reinterpret_cast<F*>(f_storage);

    setUp([instance]() {
        new (instance) F();

        if constexpr (requires { {instance->setUp()}; }) {
            instance->setUp();
        }
        if constexpr (requires { {instance->SetUp()}; }) {
            instance->SetUp();
        }
        if constexpr (requires { {instance->setup()}; }) {
            instance->setup();
        }
        if constexpr (requires { {instance->set_up()}; }) {
            instance->set_up();
        }
    });

    tearDown([instance]() {
        if constexpr (requires { {instance->tearDown()}; }) {
            instance->tearDown();
        }
        if constexpr (requires { {instance->TearDown()}; }) {
            instance->TearDown();
        }
        if constexpr (requires { {instance->tear_down()}; }) {
            instance->tear_down();
        }
        if constexpr (requires { {instance->teardown()}; }) {
            instance->teardown();
        }

        instance->~F();
    });

    test(F::MCGATest_description, [instance]() {
        instance->MCGA_TEST_BODY();
    });
}

}  // namespace mcga::test::internal

#define MCGA_TEST_INTERNAL_TEST_NAME                                           \
    MCGA_TEST_INTERNAL_CAT2(MCGA_TEST_, __LINE__)

#define MCGA_TEST_INTERNAL_TEST_REG_NAME                                       \
    MCGA_TEST_INTERNAL_CAT2(MCGA_TEST_reg_, __LINE__)

#define TEST_CASE(name)                                                        \
    namespace {                                                                \
    struct MCGA_TEST_INTERNAL_TEST_NAME {                                      \
        static void MCGA_TEST_BODY();                                          \
    };                                                                         \
    int MCGA_TEST_INTERNAL_TEST_REG_NAME                                       \
      = ::mcga::test::internal::register_test_case(                            \
        name, &MCGA_TEST_INTERNAL_TEST_NAME::MCGA_TEST_BODY);                  \
    }                                                                          \
    void MCGA_TEST_INTERNAL_TEST_NAME::MCGA_TEST_BODY()

#define TEST(description)                                                      \
    namespace {                                                                \
    struct MCGA_TEST_INTERNAL_TEST_NAME {                                      \
        static void MCGA_TEST_BODY();                                          \
    };                                                                         \
    int MCGA_TEST_INTERNAL_TEST_REG_NAME                                       \
      = ::mcga::test::internal::register_test_case("", []() -> void {          \
            ::mcga::test::test(description,                                    \
                               MCGA_TEST_INTERNAL_TEST_NAME::MCGA_TEST_BODY);  \
        });                                                                    \
    }                                                                          \
    void MCGA_TEST_INTERNAL_TEST_NAME::MCGA_TEST_BODY()

#define TEST_F(fixture, description)                                           \
    namespace {                                                                \
    struct MCGA_TEST_INTERNAL_TEST_NAME : fixture {                            \
        static inline constexpr const char* MCGATest_description               \
          = description;                                                       \
        void MCGA_TEST_BODY();                                                 \
    };                                                                         \
    int MCGA_TEST_INTERNAL_TEST_REG_NAME                                       \
      = ::mcga::test::internal::register_test_case(                            \
        #fixture,                                                              \
        &::mcga::test::internal::fixtureTestCaseBody<                          \
          MCGA_TEST_INTERNAL_TEST_NAME>);                                      \
    }                                                                          \
    void MCGA_TEST_INTERNAL_TEST_NAME::MCGA_TEST_BODY()
