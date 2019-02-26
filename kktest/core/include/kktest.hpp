#ifndef KKTEST_CORE_INCLUDE_KKTEST_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_HPP_

#include <functional>
#include <string>
#include <vector>

#define KKTEST_VERSION "1.0.0"

#if defined _WIN32 || defined __CYGWIN__
    #define KKTEST_EXPORT __declspec(dllexport)
#else
    #if __GNUC__ >= 4
        #define KKTEST_EXPORT __attribute__((visibility("default")))
    #else
        #define KKTEST_EXPORT
    #endif
#endif

namespace kktest {

typedef std::function<void()> Executable;

/** Structure defining the configuration for a test.
 *
 * Pass an instance of this class to the test() function. */
struct KKTEST_EXPORT TestConfig {
    /** Description of the test.
     *
     * A test should provide a concise, yet clear and explicit description,
     * both for future maintainers of the test and user interfaces of this
     * library. */
    std::string description = "-";

    /** Whether this test is optional.
     *
     * If only tests that have this flag marked as `true` fail during a test
     * run, the test run will still exit with code `0`, meaning a successful
     * finish. User interfaces can provide extra feedback based on this flag,
     * like marking a failed optional test in yellow instead of red (for
     * example). */
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

    /** Default constructor. */
    TestConfig() = default;

    /** Implicit constructor from a description string.
     *
     * This constructor is provided for easier use of default values for
     * #optional and #timeTicksLimit, which should be used in most cases. It is
     * implicit by design, to allow an inline string call to test(). */
    TestConfig(std::string description); // NOLINT(google-explicit-constructor)

    /** Implicit constructor from a description C-style string (see
     * TestConfig(std::string)). */
    TestConfig(const char* description); // NOLINT(google-explicit-constructor)

    /** Set the #description of the test. */
    TestConfig& setDescription(std::string description);

    /** Set the #optional flag of the test. */
    TestConfig& setOptional(bool optional);

    /** Set the #timeTicksLimit of the test. */
    TestConfig& setTimeTicksLimit(double timeTicksLimit);

    /** Set the #attempts property of the test. */
    TestConfig& setAttempts(int attempts);

    /** Set the #requiredPassedAttempts property of the test. */
    TestConfig& setRequiredPassedAttempts(int requiredPassedAttempts);
};

/** Structure defining the configuration for a group.
 *
 * Pass an instance of this class to the group() function. */
struct KKTEST_EXPORT GroupConfig {
    /** Description of the group.
     *
     * A group should provide a concise, yet clear and explicit description,
     * both for future maintainers of the group and user interfaces of this
     * library. */
    std::string description = "-";

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
    GroupConfig(std::string description); // NOLINT(google-explicit-constructor)

    /** Implicit constructor from a description C-style string (see
     * GroupConfig(std::string)). */
    GroupConfig(const char* description); // NOLINT(google-explicit-constructor)

    /** Set the #description of the group. */
    GroupConfig& setDescription(std::string description);

    /** Set the #optional flag for this group. */
    GroupConfig& setOptional(bool optional);
};

struct KKTEST_EXPORT TestCase {
    Executable exec;
    std::string name;

    TestCase(void (*exec)()); // NOLINT(google-explicit-constructor)
    TestCase(Executable exec); // NOLINT(google-explicit-constructor)
    TestCase(Executable exec, std::string name);

    virtual void run();
};

KKTEST_EXPORT void test(TestConfig config, const Executable& func);

KKTEST_EXPORT void group(GroupConfig config, const Executable& func);

KKTEST_EXPORT void setUp(const Executable& func);

KKTEST_EXPORT void tearDown(const Executable& func);

KKTEST_EXPORT void fail(const std::string& message=std::string());

KKTEST_EXPORT void expect(bool expr, const std::string& message=std::string());

KKTEST_EXPORT void init(int argc, char** argv);

KKTEST_EXPORT int run(std::vector<TestCase> tests);

KKTEST_EXPORT int initAndRun(int argc, char** argv,
                             std::vector<TestCase> tests);

}

#endif
