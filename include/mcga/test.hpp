#pragma once

#include <mcga/test/config.hpp>
#include <mcga/test/executable.hpp>
#include <mcga/test/string.hpp>

namespace mcga::test {
inline namespace MCGA_TEST_ABI_NS {

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

void group(Executable body);
void group(String description, Executable body);
void group(GroupConfig config, Executable body);

void test(Executable body);
void test(String description, Executable body);
void test(TestConfig config, Executable body);

void setUp(Executable body);
void tearDown(Executable body);
void cleanup(Executable body);

void fail(String message = String(), Context context = Context());
void skip(String message = String(), Context context = Context("Skipped"));
void expect(bool expr, Context context = Context("Expectation failed"));

}  // namespace MCGA_TEST_ABI_NS
}  // namespace mcga::test

namespace mcga::test::internal {

void register_test_case(const char* name, void (*body)(), Context context);

}  // namespace mcga::test::internal

#define TEST_CASE(name)                                                        \
    namespace {                                                                \
    struct MCGA_TEST_CAT2(mcga_test_case_, __LINE__) {                         \
        static void run();                                                     \
                                                                               \
        MCGA_TEST_CAT2(mcga_test_case_, __LINE__)() {                          \
            ::mcga::test::internal::register_test_case(                        \
              name, run, ::mcga::test::Context());                             \
        }                                                                      \
    } MCGA_TEST_CAT2(mcga_test_reg_, __LINE__);                                \
    }                                                                          \
    void MCGA_TEST_CAT2(mcga_test_case_, __LINE__)::run()
