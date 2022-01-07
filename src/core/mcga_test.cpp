#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

#include <iostream>

static mcga::test::internal::TestCase* registeredTestCasesListHead = nullptr;

namespace mcga::test::internal {

MCGA_TEST_EXPORT extern "C" void
  mcga_test_register_test_case(TestCase* testCase) {
    testCase->next = registeredTestCasesListHead;
    registeredTestCasesListHead = testCase;
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_test(TestConfig config,
                                                         Executable body) {
    Driver::Instance()->addTest(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_group(GroupConfig config,
                                                          Executable body) {
    Driver::Instance()->addGroup(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_set_up(Executable body) {
    Driver::Instance()->addSetUp(std::move(body));
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_tear_down(Executable body) {
    Driver::Instance()->addTearDown(std::move(body));
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_failure(String message,
                                                            Context context) {
    Driver::Instance()->addFailure(std::string(message.c_str()),
                                   std::move(context));
}

MCGA_TEST_EXPORT extern "C" void mcga_test_register_cleanup(Executable exec) {
    Driver::Instance()->addCleanup(std::move(exec));
}

}  // namespace mcga::test::internal

namespace mcga::test {

// Intentionally not exported, only used internally within the library.
std::vector<internal::TestCase*> getTestCases() {
    std::vector<internal::TestCase*> testCasesRegistered;
    for (auto testCase = registeredTestCasesListHead; testCase != nullptr;
         testCase = testCase->next) {
        testCasesRegistered.push_back(testCase);
    }
    std::reverse(testCasesRegistered.begin(), testCasesRegistered.end());
    return testCasesRegistered;
}

}  // namespace mcga::test
