#pragma once

#include <functional>
#include <string>

namespace mcga::test {

/** Type of function widely used throughout the library.
 *
 * This defines the simple concept of a function that can be executed with no
 * parameters and returns nothing. TestCase, test, group, setUp and tearDown are
 * all constructed from Executable. */
using Executable = std::function<void()>;

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
void test(TestConfig config, Executable body);

/** Convenience function for defining an anonymous test. */
inline void test(Executable body) {
    test(TestConfig(), std::move(body));
}

/** Convenience function for defining an optional test. */
inline void optionalTest(TestConfig config, Executable body) {
    config.setOptional();
    test(std::move(config), std::move(body));
}

/** Convenience function for defining an anonymous optional test. */
inline void optionalTest(Executable body) {
    optionalTest(TestConfig(), std::move(body));
}

/** Convenience function for defining a test that will be executed multiple
 * times, and all executions must pass for the test to be marked as `passed`. */
inline void
  multiRunTest(TestConfig config, std::size_t numRuns, Executable body) {
    config.setRequiredPassedAttempts(numRuns);
    config.setAttempts(numRuns);
    test(std::move(config), std::move(body));
}

/** Convenience function for defining an anonymous test that will be executed
 * multiple times, and all executions must pass for the test to be marked as
 * `passed`. */
inline void multiRunTest(std::size_t numRuns, Executable body) {
    multiRunTest(TestConfig(), numRuns, std::move(body));
}

/** Convenience function for defining an optional test that will be executed
 * multiple times, and all executions must pass for the test to be marked as
 * `passed`. */
inline void optionalMultiRunTest(TestConfig config,
                                 std::size_t numRuns,
                                 Executable body) {
    config.setOptional();
    multiRunTest(std::move(config), numRuns, std::move(body));
}

/** Convenience function for defining an anonymous optional test that will be
 * executed multiple times, and all executions must pass for the test to be
 * marked as `passed`. */
inline void optionalMultiRunTest(std::size_t numRuns, Executable body) {
    optionalMultiRunTest(TestConfig(), numRuns, std::move(body));
}

/** Convenience function for defining a test that will be retried multiple
 * times, and only one needs to pass for the test to be marked as `passed`. */
inline void
  retryTest(TestConfig config, std::size_t attempts, Executable body) {
    config.setAttempts(attempts);
    config.setRequiredPassedAttempts(1);
    test(std::move(config), std::move(body));
}

/** Convenience function for defining an anonymous test that will be retried
 * multiple times, and only one needs to pass for the test to be marked as
 * `passed`. */
inline void retryTest(std::size_t numAttempts, Executable body) {
    retryTest(TestConfig(), numAttempts, std::move(body));
}

/** Convenience function for defining an optional test that will be retried
 * multiple times, and only one needs to pass for the test to be marked as
 * `passed`. */
inline void
  optionalRetryTest(TestConfig config, std::size_t attempts, Executable body) {
    config.setOptional();
    retryTest(std::move(config), attempts, std::move(body));
}

/** Convenience function for defining an optional anonymous test that will be
 * retried multiple times, and only one needs to pass for the test to be marked
 * as `passed`. */
inline void optionalRetryTest(std::size_t numAttempts, Executable body) {
    optionalRetryTest(TestConfig(), numAttempts, std::move(body));
}

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
void group(GroupConfig config, const Executable& body);

/** Convenience function for defining an anonymous group. */
inline void group(const Executable& body) {
    group(GroupConfig(), body);
}

/** Convenience function for defining an optional group. */
inline void optionalGroup(GroupConfig config, const Executable& body) {
    config.setOptional();
    group(std::move(config), body);
}

/** Convenience function for defining an anonymous optional group. */
inline void optionalGroup(const Executable& body) {
    optionalGroup(GroupConfig(), body);
}

/** Main mechanism for defining setUp functions, to be executed before every
 * test in a group. */
void setUp(Executable func);

/** Main mechanism for defining tearDown functions, to be executed after every
 * test in a group. */
void tearDown(Executable func);

/** Main mechanism for marking a test as failed.
 *
 * Calling this function inside the main testing thread disrupts the test
 * completely. Calling it in a separate thread will still mark the test as
 * failed, but will not interrupt any thread. */
void fail(const std::string& message = std::string());

/** Convenience function for marking a test as failed if a boolean expression
 * does not evaluate to `true`. */
inline void expect(bool expr, const std::string& message = std::string()) {
    if (!expr) {
        fail(message);
    }
}

struct TestCase {
    Executable body;

    std::string name;

    TestCase(Executable body, std::string name);
};

}  // namespace mcga::test

#define TEST_CASE(UNIQUE_NAME, DESC)                                           \
    void UNIQUE_NAME##_func();                                                 \
    static mcga::test::TestCase UNIQUE_NAME##_case(UNIQUE_NAME##_func, DESC);  \
    void UNIQUE_NAME##_func()
