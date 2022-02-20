#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

namespace mcga::test::internal {

static TestCase* testCasesHead = nullptr;
static TestCase* testCasesTail = nullptr;

TestCase* getTestCasesHead() {
    return testCasesHead;
}

MCGA_TEST_EXPORT void register_test_case(TestCase* test_case) {
    list_push_back(testCasesHead, testCasesTail, test_case);
}

MCGA_TEST_EXPORT void register_test_in_fixture(FixtureData* fixture,
                                               const char* fixture_name,
                                               FixtureTest* fixture_test) {
    fixture->test_case.name = fixture_name;
    list_push_back(fixture->head, fixture->tail, fixture_test);
}

}  // namespace mcga::test::internal

namespace mcga::test {
inline namespace MCGA_TEST_INTERNAL_ABI_NS {

MCGA_TEST_EXPORT void group(GroupConfig config, Executable body) {
    Driver::Instance()->addGroup(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT void test(TestConfig config, Executable body) {
    Driver::Instance()->addTest(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT void setUp(Executable body) {
    Driver::Instance()->addSetUp(std::move(body));
}

MCGA_TEST_EXPORT void tearDown(Executable body) {
    Driver::Instance()->addTearDown(std::move(body));
}

MCGA_TEST_EXPORT void cleanup(Executable body) {
    Driver::Instance()->addCleanup(std::move(body));
}

MCGA_TEST_EXPORT void fail(String message, Context context, String verb) {
    Driver::Instance()->addFailure(
      Test::ExecutionInfo{.status = Test::ExecutionInfo::FAILED,
                          .verb = std::move(verb),
                          .message = std::move(message),
                          .context = context});
}

MCGA_TEST_EXPORT void skip(String message, Context context) {
    Driver::Instance()->addFailure(
      Test::ExecutionInfo{.status = Test::ExecutionInfo::SKIPPED,
                          .verb = "Skipped",
                          .message = std::move(message),
                          .context = context});
}

}  // namespace MCGA_TEST_INTERNAL_ABI_NS
}  // namespace mcga::test
