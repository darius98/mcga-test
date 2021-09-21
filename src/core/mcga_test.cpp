#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

#include <iostream>

namespace mcga::test {

namespace internal {

MCGA_TEST_EXPORT void
  registerTest(TestConfig config, Executable body, Context context) {
    Driver::Instance()->addTest(
      std::move(config), std::move(body), std::move(context));
}

MCGA_TEST_EXPORT void
  registerGroup(GroupConfig config, const Executable& body, Context context) {
    Driver::Instance()->addGroup(std::move(config), body, std::move(context));
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

}  // namespace mcga::test
