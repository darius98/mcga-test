#pragma once

#include <functional>
#include <string>

#include "mcga/internal/source_location.hpp"

namespace mcga::test {

/** Type of function widely used throughout the library.
 *
 * This defines the simple concept of a function that can be executed with no
 * parameters and returns nothing. TestCase, test, group, setUp and tearDown are
 * all constructed from Executable. */
using Executable = std::function<void()>;

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

/** Structure defining the configuration for a test.
 *
 * Pass an instance of this class to the test() function. */
struct TestConfig {
    /** Description of the test.
     *
     * A test should provide a concise, yet clear and explicit description, both
     * for future maintainers of the test and UIs of this library. */
    std::string description = "(anonymous test)";

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

    Context context;

    /** Default constructor. */
    TestConfig() = default;

    /** Implicit constructor from a description string.
     *
     * This constructor is provided for easier use of default values for
     * #optional and #timeTicksLimit, which should be used in most cases. It is
     * implicit by design, to allow an inline string call to test(). */
    TestConfig(std::string description): description(std::move(description)) {
    }

    /** Implicit constructor from a description C-style string (see
     * TestConfig(std::string)). */
    TestConfig(const char* description): description(description) {
    }

    /** Set the #description of the test. */
    TestConfig& setDescription(std::string _description) {
        description = std::move(_description);
        return *this;
    }

    /** Set the #optional flag of the test. */
    TestConfig& setOptional(bool _optional = true) {
        optional = _optional;
        return *this;
    }

    /** Set the #timeTicksLimit of the test. */
    TestConfig& setTimeTicksLimit(double _timeTicksLimit) {
        timeTicksLimit = _timeTicksLimit;
        return *this;
    }

    /** Set the #attempts property of the test. */
    TestConfig& setAttempts(std::size_t _attempts) {
        attempts = _attempts;
        return *this;
    }

    /** Set the #requiredPassedAttempts property of the test. */
    TestConfig& setRequiredPassedAttempts(std::size_t _requiredPassedAttempts) {
        requiredPassedAttempts = _requiredPassedAttempts;
        return *this;
    }
};

/** Main mechanism for defining tests. */
void test(TestConfig config, Executable body, Context context = Context());

/** Convenience function for defining an anonymous test. */
void test(Executable body, Context context = Context());

/** Convenience function for defining an optional test. */
void optionalTest(TestConfig config,
                  Executable body,
                  Context context = Context());

/** Convenience function for defining an anonymous optional test. */
void optionalTest(Executable body, Context context = Context());

/** Convenience function for defining a test that will be executed multiple
 * times, and all executions must pass for the test to be marked as `passed`. */
void multiRunTest(TestConfig config,
                  std::size_t numRuns,
                  Executable body,
                  Context context = Context());

/** Convenience function for defining an anonymous test that will be executed
 * multiple times, and all executions must pass for the test to be marked as
 * `passed`. */
void multiRunTest(std::size_t numRuns,
                  Executable body,
                  Context context = Context());

/** Convenience function for defining an optional test that will be executed
 * multiple times, and all executions must pass for the test to be marked as
 * `passed`. */
void optionalMultiRunTest(TestConfig config,
                          std::size_t numRuns,
                          Executable body,
                          Context context = Context());

/** Convenience function for defining an anonymous optional test that will be
 * executed multiple times, and all executions must pass for the test to be
 * marked as `passed`. */
void optionalMultiRunTest(std::size_t numRuns,
                          Executable body,
                          Context context = Context());

/** Convenience function for defining a test that will be retried multiple
 * times, and only one needs to pass for the test to be marked as `passed`. */
void retryTest(TestConfig config,
               std::size_t attempts,
               Executable body,
               Context context = Context());

/** Convenience function for defining an anonymous test that will be retried
 * multiple times, and only one needs to pass for the test to be marked as
 * `passed`. */
void retryTest(std::size_t numAttempts,
               Executable body,
               Context context = Context());

/** Convenience function for defining an optional test that will be retried
 * multiple times, and only one needs to pass for the test to be marked as
 * `passed`. */
void optionalRetryTest(TestConfig config,
                       std::size_t attempts,
                       Executable body,
                       Context context = Context());

/** Convenience function for defining an optional anonymous test that will be
 * retried multiple times, and only one needs to pass for the test to be marked
 * as `passed`. */
void optionalRetryTest(std::size_t numAttempts,
                       Executable body,
                       Context context = Context());

/** Structure defining the configuration for a group.
 *
 * Pass an instance of this class to the group() function. */
struct GroupConfig {
    /** Description of the group.
     *
     * A group should provide a concise, yet clear and explicit description,
     * both for future maintainers of the group and user interfaces of this
     * library. */
    std::string description = "(anonymous group)";

    /** Flag that marks this whole group as optional.
     *
     * This is equivalent to marking each individual test inside the group and
     * all its subgroups as optional. */
    bool optional = false;

    Context context;

    /** Default constructor. */
    GroupConfig() = default;

    /** Implicit constructor from a description string.
     *
     * This constructor is provided for easier use of default values for
     * all the other properties, which should be used in most cases. It is
     * implicit by design, to allow an inline string call to group(). */
    GroupConfig(std::string description): description(std::move(description)) {
    }

    /** Implicit constructor from a description C-style string (see
     * GroupConfig(std::string)). */
    GroupConfig(const char* description): description(description) {
    }

    /** Set the #description of the group. */
    GroupConfig& setDescription(std::string _description) {
        description = std::move(_description);
        return *this;
    }

    /** Set the #optional flag for this group. */
    GroupConfig& setOptional(bool _optional = true) {
        optional = _optional;
        return *this;
    }
};

/** Main mechanism for defining groups. */
void group(GroupConfig config,
           const Executable& body,
           Context context = Context());

/** Convenience function for defining an anonymous group. */
void group(const Executable& body, Context context = Context());

/** Convenience function for defining an optional group. */
void optionalGroup(GroupConfig config,
                   const Executable& body,
                   Context context = Context());

/** Convenience function for defining an anonymous optional group. */
void optionalGroup(const Executable& body, Context context = Context());

/** Main mechanism for defining setUp functions, to be executed before every
 * test in a group. */
void setUp(Executable func, Context context = Context());

/** Main mechanism for defining tearDown functions, to be executed after every
 * test in a group. */
void tearDown(Executable func, Context context = Context());

/** Main mechanism for marking a test as failed.
 *
 * Calling this function inside the main testing thread disrupts the test
 * completely. Calling it in a separate thread will still mark the test as
 * failed, but will not interrupt any thread. */
void fail(const std::string& message = std::string(),
          Context context = Context());

/** Convenience function for marking a test as failed if a boolean expression
 * does not evaluate to `true`. */
void expect(bool expr, Context context = Context());

struct TestCase {
    Executable body;

    std::string name;

    Context context;

    TestCase(Executable body, std::string name, Context context);
};

}  // namespace mcga::test

#define TEST_CASE(UNIQUE_NAME, DESC)                                           \
    void UNIQUE_NAME##_func();                                                 \
    static mcga::test::TestCase UNIQUE_NAME##_case(                            \
      UNIQUE_NAME##_func, DESC, ::mcga::test::Context());                      \
    void UNIQUE_NAME##_func()
