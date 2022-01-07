#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

#include <iostream>

static mcga::test::internal::TestCase* registeredTestCasesListHead = nullptr;

namespace mcga::test::internal {

MCGA_TEST_EXPORT extern "C" void mcga_test_register_test_case(TestCase* testCase) {
    testCase->next = registeredTestCasesListHead;
    registeredTestCasesListHead = testCase;
}

MCGA_TEST_EXPORT extern "C" void
  mcga_test_register_test(TestConfig config, Executable body, Context context) {
    Driver::Instance()->addTest(
      std::move(config), std::move(body), std::move(context));
}

MCGA_TEST_EXPORT extern "C" void
  mcga_test_register_group(GroupConfig config, const Executable& body, Context context) {
    Driver::Instance()->addGroup(std::move(config), body, std::move(context));
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_set_up(Executable body,
                                               Context context) {
    Driver::Instance()->addSetUp(
      UserTestExecutable{std::move(body), std::move(context)});
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_tear_down(Executable body,
                                                  Context context) {
    Driver::Instance()->addTearDown(
      UserTestExecutable{std::move(body), std::move(context)});
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_failure(std::string message, Context context) {
    Driver::Instance()->addFailure(std::move(message), std::move(context));
}

// Intentionally not exported, only used internally within the library.
std::vector<TestCase*> getTestCases() {
    std::vector<TestCase*> testCasesRegistered;
    for (auto testCase = registeredTestCasesListHead; testCase != nullptr;
         testCase = testCase->next) {
        testCasesRegistered.push_back(testCase);
    }
    std::reverse(testCasesRegistered.begin(), testCasesRegistered.end());
    return testCasesRegistered;
}

}  // namespace mcga::test
