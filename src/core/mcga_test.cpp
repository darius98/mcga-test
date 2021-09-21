#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

#include <iostream>

namespace mcga::test {

namespace internal {

MCGA_TEST_EXPORT void MultiRunTestApi::operator()(std::size_t numRuns,
                                                  TestConfig config,
                                                  Executable body,
                                                  Context context) const {
    config.attempts = numRuns;
    config.requiredPassedAttempts = numRuns;
    config.optional = isOptional;
    Driver::Instance()->addTest(
      std::move(config), std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void MultiRunTestApi::operator()(std::size_t numRuns,
                                                  std::string description,
                                                  Executable body,
                                                  Context context) const {
    (*this)(numRuns,
            TestConfig{.description = std::move(description)},
            std::move(body),
            std::move(context));
}

MCGA_TEST_EXPORT void MultiRunTestApi::operator()(std::size_t numRuns,
                                                  Executable body,
                                                  Context context) const {
    (*this)(numRuns, TestConfig{}, std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void RetryTestApi::operator()(std::size_t attempts,
                                               TestConfig config,
                                               Executable body,
                                               Context context) const {
    config.attempts = attempts;
    config.requiredPassedAttempts = 1;
    config.optional = isOptional;
    Driver::Instance()->addTest(
      std::move(config), std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void RetryTestApi::operator()(std::size_t attempts,
                                               std::string description,
                                               Executable body,
                                               Context context) const {
    (*this)(attempts,
            TestConfig{.description = std::move(description)},
            std::move(body),
            std::move(context));
}

MCGA_TEST_EXPORT void RetryTestApi::operator()(std::size_t numAttempts,
                                               Executable body,
                                               Context context) const {
    (*this)(numAttempts, TestConfig(), std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void OptionalTestApi::operator()(TestConfig config,
                                                  Executable body,
                                                  Context context) const {
    config.optional = isOptional;
    Driver::Instance()->addTest(
      std::move(config), std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void OptionalTestApi::operator()(std::string description,
                                                  Executable body,
                                                  Context context) const {
    (*this)(TestConfig{.description = std::move(description)},
            std::move(body),
            std::move(context));
}

MCGA_TEST_EXPORT void OptionalTestApi::operator()(Executable body,
                                                  Context context) const {
    (*this)(TestConfig(), std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void OptionalGroupApi::operator()(GroupConfig config,
                                                   const Executable& body,
                                                   Context context) const {
    config.optional = isOptional;
    Driver::Instance()->addGroup(std::move(config), body, std::move(context));
}

MCGA_TEST_EXPORT void OptionalGroupApi::operator()(std::string description,
                                                   const Executable& body,
                                                   Context context) const {
    (*this)(GroupConfig{.description = std::move(description)},
            body,
            std::move(context));
}

MCGA_TEST_EXPORT void OptionalGroupApi::operator()(const Executable& body,
                                                   Context context) const {
    (*this)(GroupConfig(), body, std::move(context));
}

MCGA_TEST_EXPORT void SetUpApi::operator()(Executable func,
                                           Context context) const {
    Driver::Instance()->addSetUp(
      UserTestExecutable{std::move(func), std::move(context)});
}

MCGA_TEST_EXPORT void TearDownApi::operator()(Executable func,
                                              Context context) const {
    Driver::Instance()->addTearDown(
      UserTestExecutable{std::move(func), std::move(context)});
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
    std::vector<TestCase*> testCasesRegistered;
    for (auto testCase = registeredTestCasesListHead; testCase != nullptr;
         testCase = testCase->next) {
        testCasesRegistered.push_back(testCase);
    }
    std::reverse(testCasesRegistered.begin(), testCasesRegistered.end());
    return testCasesRegistered;
}

}  // namespace internal

MCGA_TEST_EXPORT void fail(const std::string& message, Context context) {
    Driver::Instance()->addFailure(message, std::move(context));
}

MCGA_TEST_EXPORT void expect(bool expr, Context context) {
    if (!expr) {
        context.verb = "Expectation failed";
        fail("", std::move(context));
    }
}

}  // namespace mcga::test
