#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

namespace mcga::test {

MCGA_TEST_EXPORT void test(TestConfig config, Executable body) {
    Driver::Instance()->addTest(std::move(config), std::move(body));
}

MCGA_TEST_EXPORT void group(GroupConfig config, const Executable& func) {
    Driver::Instance()->addGroup(std::move(config), func);
}

MCGA_TEST_EXPORT void setUp(Executable func) {
    Driver::Instance()->addSetUp(std::move(func));
}

MCGA_TEST_EXPORT void tearDown(Executable func) {
    Driver::Instance()->addTearDown(std::move(func));
}

MCGA_TEST_EXPORT void fail(const std::string& message) {
    Driver::Instance()->addFailure(message);
}

}  // namespace mcga::test
