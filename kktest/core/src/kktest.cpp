#include "kktest/core/include/kktest.hpp"

#include "kktest/core/src/driver.hpp"
#include "kktest/core/src/export.hpp"

using namespace std;

namespace kktest {

KKTEST_EXPORT void test(TestConfig config, Executable func) {
    Driver::Instance()->addTest(move(config), move(func));
}

KKTEST_EXPORT void group(GroupConfig config, const Executable& func) {
    Driver::Instance()->addGroup(move(config), func);
}

KKTEST_EXPORT void setUp(Executable func) {
    Driver::Instance()->addSetUp(move(func));
}

KKTEST_EXPORT void tearDown(Executable func) {
    Driver::Instance()->addTearDown(move(func));
}

KKTEST_EXPORT void fail(const string& message) {
    throw ExpectationFailed(message);
}

}
