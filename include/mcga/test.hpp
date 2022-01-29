#pragma once

#include <mcga/test/config.hpp>
#include <mcga/test/executable.hpp>
#include <mcga/test/string.hpp>

namespace mcga::test {

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

namespace MCGA_TEST_ABI_NS {

void register_test_case(const char* name, void (*body)(), Context context);

void register_test(TestConfig config, Executable body);

void register_group(GroupConfig config, Executable body);

void register_set_up(Executable body);

void register_tear_down(Executable body);

void register_interrupt(bool isFail, String message, Context context);

void register_cleanup(Executable exec);

}  // namespace MCGA_TEST_ABI_NS

inline void group(Executable body) {
    MCGA_TEST_ABI_NS::register_group({}, internal::move(body));
}

inline void group(String description, Executable body) {
    MCGA_TEST_ABI_NS::register_group(
      {.description = internal::move(description)}, internal::move(body));
}

inline void group(GroupConfig config, Executable body) {
    MCGA_TEST_ABI_NS::register_group(internal::move(config),
                                     internal::move(body));
}

inline void test(Executable body) {
    MCGA_TEST_ABI_NS::register_test({}, internal::move(body));
}

inline void test(String description, Executable body) {
    MCGA_TEST_ABI_NS::register_test(
      {.description = internal::move(description)}, internal::move(body));
}

inline void test(TestConfig config, Executable body) {
    MCGA_TEST_ABI_NS::register_test(internal::move(config),
                                    internal::move(body));
}

inline void setUp(Executable body) {
    MCGA_TEST_ABI_NS::register_set_up(internal::move(body));
}

inline void cleanup(Executable body) {
    MCGA_TEST_ABI_NS::register_cleanup(internal::move(body));
}

inline void tearDown(Executable body) {
    MCGA_TEST_ABI_NS::register_tear_down(internal::move(body));
}

inline void fail(String message = String(), Context context = Context()) {
    MCGA_TEST_ABI_NS::register_interrupt(
      true, internal::move(message), context);
}

inline void skip(String message = String(),
                 Context context = Context("Skipped")) {
    MCGA_TEST_ABI_NS::register_interrupt(
      false, internal::move(message), context);
}

inline void expect(bool expr, Context context = Context("Expectation failed")) {
    if (!expr) {
        fail("", context);
    }
}

}  // namespace mcga::test

#define TEST_CASE(name)                                                        \
    namespace {                                                                \
    struct MCGA_TEST_CAT2(mcga_test_case_, __LINE__) {                         \
        static void run();                                                     \
                                                                               \
        MCGA_TEST_CAT2(mcga_test_case_, __LINE__)() {                          \
            ::mcga::test::MCGA_TEST_ABI_NS::register_test_case(                \
              name, run, ::mcga::test::Context());                             \
        }                                                                      \
    } MCGA_TEST_CAT2(mcga_test_reg_, __LINE__);                                \
    }                                                                          \
    void MCGA_TEST_CAT2(mcga_test_case_, __LINE__)::run()
