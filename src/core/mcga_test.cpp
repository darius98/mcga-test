#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

namespace mcga::test {

MCGA_TEST_EXPORT void
  test(TestConfig config, Executable body, Context context) {
    config.context = std::move(context);
    Driver::Instance()->addTest(std::move(config), std::move(body));
}

/** Convenience function for defining an anonymous test. */
MCGA_TEST_EXPORT void test(Executable body, Context context) {
    test(TestConfig(), std::move(body), std::move(context));
}

/** Convenience function for defining an optional test. */
MCGA_TEST_EXPORT void
  optionalTest(TestConfig config, Executable body, Context context) {
    config.setOptional();
    test(std::move(config), std::move(body), std::move(context));
}

/** Convenience function for defining an anonymous optional test. */
MCGA_TEST_EXPORT void optionalTest(Executable body, Context context) {
    optionalTest(TestConfig(), std::move(body), std::move(context));
}

/** Convenience function for defining a test that will be executed multiple
 * times, and all executions must pass for the test to be marked as `passed`. */
MCGA_TEST_EXPORT void multiRunTest(TestConfig config,
                                   std::size_t numRuns,
                                   Executable body,
                                   Context context) {
    config.setRequiredPassedAttempts(numRuns);
    config.setAttempts(numRuns);
    test(std::move(config), std::move(body), std::move(context));
}

/** Convenience function for defining an anonymous test that will be executed
 * multiple times, and all executions must pass for the test to be marked as
 * `passed`. */
MCGA_TEST_EXPORT void
  multiRunTest(std::size_t numRuns, Executable body, Context context) {
    multiRunTest(TestConfig(), numRuns, std::move(body), std::move(context));
}

/** Convenience function for defining an optional test that will be executed
 * multiple times, and all executions must pass for the test to be marked as
 * `passed`. */
MCGA_TEST_EXPORT void optionalMultiRunTest(TestConfig config,
                                           std::size_t numRuns,
                                           Executable body,
                                           Context context) {
    config.setOptional();
    multiRunTest(
      std::move(config), numRuns, std::move(body), std::move(context));
}

/** Convenience function for defining an anonymous optional test that will be
 * executed multiple times, and all executions must pass for the test to be
 * marked as `passed`. */
MCGA_TEST_EXPORT void
  optionalMultiRunTest(std::size_t numRuns, Executable body, Context context) {
    optionalMultiRunTest(
      TestConfig(), numRuns, std::move(body), std::move(context));
}

/** Convenience function for defining a test that will be retried multiple
 * times, and only one needs to pass for the test to be marked as `passed`. */
MCGA_TEST_EXPORT void retryTest(TestConfig config,
                                std::size_t attempts,
                                Executable body,
                                Context context) {
    config.setAttempts(attempts);
    config.setRequiredPassedAttempts(1);
    test(std::move(config), std::move(body), std::move(context));
}

/** Convenience function for defining an anonymous test that will be retried
 * multiple times, and only one needs to pass for the test to be marked as
 * `passed`. */
MCGA_TEST_EXPORT void
  retryTest(std::size_t numAttempts, Executable body, Context context) {
    retryTest(TestConfig(), numAttempts, std::move(body), std::move(context));
}

/** Convenience function for defining an optional test that will be retried
 * multiple times, and only one needs to pass for the test to be marked as
 * `passed`. */
MCGA_TEST_EXPORT void optionalRetryTest(TestConfig config,
                                        std::size_t attempts,
                                        Executable body,
                                        Context context) {
    config.setOptional();
    retryTest(std::move(config), attempts, std::move(body), std::move(context));
}

/** Convenience function for defining an optional anonymous test that will be
 * retried multiple times, and only one needs to pass for the test to be marked
 * as `passed`. */
MCGA_TEST_EXPORT void
  optionalRetryTest(std::size_t numAttempts, Executable body, Context context) {
    optionalRetryTest(
      TestConfig(), numAttempts, std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void
  group(GroupConfig config, const Executable& func, Context context) {
    config.context = std::move(context);
    Driver::Instance()->addGroup(std::move(config), func);
}

MCGA_TEST_EXPORT void group(const Executable& body, Context context) {
    group(GroupConfig(), body, std::move(context));
}

MCGA_TEST_EXPORT void
  optionalGroup(GroupConfig config, const Executable& body, Context context) {
    config.setOptional();
    group(std::move(config), body, std::move(context));
}

MCGA_TEST_EXPORT void optionalGroup(const Executable& body, Context context) {
    optionalGroup(GroupConfig(), body, std::move(context));
}

MCGA_TEST_EXPORT void setUp(Executable func, Context context) {
    Driver::Instance()->addSetUp(
      UserTestExecutable{std::move(func), std::move(context)});
}

MCGA_TEST_EXPORT void tearDown(Executable func, Context context) {
    Driver::Instance()->addTearDown(
      UserTestExecutable{std::move(func), std::move(context)});
}

MCGA_TEST_EXPORT void fail(const std::string& message, Context context) {
    Driver::Instance()->addFailure(message, std::move(context));
}

MCGA_TEST_EXPORT void expect(bool expr, Context context) {
    if (!expr) {
        context.verb = "Expectation failed";
        fail("", std::move(context));
    }
}

static TestCase* registeredTestCasesListHead = nullptr;

MCGA_TEST_EXPORT
TestCase::TestCase(void (*body)(),
                   const char* name,
                   internal::source_location location) noexcept
        : body(body), name(name), location(location),
          next(registeredTestCasesListHead) {
    registeredTestCasesListHead = this;
}

MCGA_TEST_EXPORT
std::vector<TestCase*> getTestCases() {
    std::vector<mcga::test::TestCase*> testCasesRegistered;
    for (auto testCase = mcga::test::registeredTestCasesListHead;
         testCase != nullptr;
         testCase = testCase->next) {
        testCasesRegistered.push_back(testCase);
    }
    std::reverse(testCasesRegistered.begin(), testCasesRegistered.end());
    return testCasesRegistered;
}

}  // namespace mcga::test
