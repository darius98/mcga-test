#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

namespace mcga::test::internal {

static TestCaseList testCases;

MCGA_TEST_EXPORT int
  register_test_case(const char* name, void (*body)(), Context context) {
    if (!testCases.push_back(TestCase{name, body, context})) {
        // TODO: Somehow emit a warning?
    }
    return 0;
}

}  // namespace mcga::test::internal

namespace mcga::test {
inline namespace MCGA_TEST_INTERNAL_ABI_NS {

MCGA_TEST_EXPORT void group(Executable body) {
    Driver::Instance()->addGroup({}, std::move(body));
}

MCGA_TEST_EXPORT void group(String description, Executable body) {
    Driver::Instance()->addGroup({.description = std::move(description)},
                                 std::move(body));
}

MCGA_TEST_EXPORT void group(GroupConfig config, Executable body) {
    Driver::Instance()->addGroup(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT void test(Executable body) {
    Driver::Instance()->addTest({}, std::move(body));
}

MCGA_TEST_EXPORT void test(String description, Executable body) {
    Driver::Instance()->addTest({.description = std::move(description)},
                                std::move(body));
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

MCGA_TEST_EXPORT void fail(String message, Context context) {
    Driver::Instance()->addFailure(
      Test::ExecutionInfo{.status = Test::ExecutionInfo::FAILED,
                          .message = std::move(message),
                          .context = context});
}

MCGA_TEST_EXPORT void skip(String message, Context context) {
    Driver::Instance()->addFailure(
      Test::ExecutionInfo{.status = Test::ExecutionInfo::SKIPPED,
                          .message = std::move(message),
                          .context = context});
}

MCGA_TEST_EXPORT void expect(bool expr, Context context) {
    if (!expr) {
        fail("", context);
    }
}

}  // namespace MCGA_TEST_INTERNAL_ABI_NS
}  // namespace mcga::test

namespace mcga::test {

const TestCaseList* getTestCasesList() {
    return &internal::testCases;
}

}  // namespace mcga::test
