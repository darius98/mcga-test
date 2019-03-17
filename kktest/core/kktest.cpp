#include "kktest/core/include/kktest.hpp"

#include "kktest/core/driver.hpp"
#include "kktest/core/export.hpp"

using namespace std;

namespace kktest {

KKTEST_EXPORT void test(TestConfig config, Executable body) {
    Driver::Instance()->addTest(move(config), move(body));
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
    Driver::Instance()->addFailure(message);
}

}
