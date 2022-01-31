#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"
#include "test_case.hpp"

namespace mcga::test {

alignas(TestCaseList) static unsigned char testCasesListStorage[sizeof(TestCaseList)];
static constinit bool testCaseListInit = false;

TestCaseList* getTestCasesList() {
    auto ptr = reinterpret_cast<TestCaseList*>(testCasesListStorage);
    if (!testCaseListInit) {
        new (ptr) TestCaseList();
        testCaseListInit = true;
    }
    return ptr;
}

}

namespace mcga::test::internal {

MCGA_TEST_EXPORT int
  register_test_case(const char* name, void (*body)(), Context context) {
    if (!getTestCasesList()->push_back(TestCase{name, body, context})) {
        // TODO: Somehow emit a warning?
    }
    return 0;
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
