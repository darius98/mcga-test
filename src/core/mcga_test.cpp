#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

static mcga::test::TestCase* registeredTestCasesListHead = nullptr;
static mcga::test::TestCase* registeredTestCasesListTail = nullptr;

namespace mcga::test::MCGA_TEST_ABI_NS {

MCGA_TEST_EXPORT void register_test_case(TestCase* testCase) noexcept {
    if (registeredTestCasesListTail != nullptr) {
        registeredTestCasesListTail->next = testCase;
    } else {
        registeredTestCasesListHead = testCase;
    }
    registeredTestCasesListTail = testCase;
}

MCGA_TEST_EXPORT void register_test(TestConfig config, Executable body) {
    Driver::Instance()->addTest(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT void register_group(GroupConfig config, Executable body) {
    Driver::Instance()->addGroup(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT void register_set_up(Executable body) {
    Driver::Instance()->addSetUp(std::move(body));
}

MCGA_TEST_EXPORT void register_tear_down(Executable body) {
    Driver::Instance()->addTearDown(std::move(body));
}

MCGA_TEST_EXPORT void
  register_interrupt(bool isFail, String message, Context context) {
    const auto status
      = isFail ? Test::ExecutionInfo::FAILED : Test::ExecutionInfo::SKIPPED;
    Driver::Instance()->addFailure(Test::ExecutionInfo{
      .status = status, .message = std::move(message), .context = context});
}

MCGA_TEST_EXPORT void register_cleanup(Executable exec) {
    Driver::Instance()->addCleanup(std::move(exec));
}

}  // namespace mcga::test::internal

namespace mcga::test {

TestCase* getTestCasesListHead() {
    return registeredTestCasesListHead;
}

}  // namespace mcga::test
