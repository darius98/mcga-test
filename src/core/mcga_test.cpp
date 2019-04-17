#include "mcga/test.hpp"

#include "driver.hpp"
#include "export.hpp"

using namespace std;

namespace mcga::test {

MCGA_TEST_EXPORT void test(TestConfig config, Executable body) {
    Driver::Instance()->addTest(move(config), move(body));
}

MCGA_TEST_EXPORT void group(GroupConfig config, const Executable& func) {
    Driver::Instance()->addGroup(move(config), func);
}

MCGA_TEST_EXPORT void setUp(Executable func) {
    Driver::Instance()->addSetUp(move(func));
}

MCGA_TEST_EXPORT void tearDown(Executable func) {
    Driver::Instance()->addTearDown(move(func));
}

MCGA_TEST_EXPORT void fail(const string& message) {
    Driver::Instance()->addFailure(message);
}

}  // namespace mcga::test
