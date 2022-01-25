#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

#include <cstdlib>
#include <cstring>

static mcga::test::TestCase* registeredTestCasesListHead = nullptr;
static mcga::test::TestCase* registeredTestCasesListTail = nullptr;

namespace mcga::test::internal {

MCGA_TEST_EXPORT const char* duplicate_str(const char* data) {
    const auto len = std::strlen(data);
    const auto dup = static_cast<char*>(std::malloc(len + 1));
    std::strcpy(dup, data);
    return dup;
}

MCGA_TEST_EXPORT void delete_str(const char* data) {
    std::free((void*)data);
}

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
