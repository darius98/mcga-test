#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

namespace mcga::test::MCGA_TEST_ABI_NS {

static TestCaseList testCases;

MCGA_TEST_EXPORT void
  register_test_case(const char* name, void (*body)(), Context context) {
    testCases.push_back(TestCase{name, body, context});
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

}  // namespace mcga::test::MCGA_TEST_ABI_NS

namespace mcga::test {

const TestCaseList* getTestCasesList() {
    return &MCGA_TEST_ABI_NS::testCases;
}

}  // namespace mcga::test
