#include "kktest/core/include/kktest.hpp"

#include "kktest/core/src/driver.hpp"

using namespace std;

namespace kktest {

void test(TestConfig config, Executable func) {
    Driver::Instance()->addTest(move(config), move(func));
}

void group(GroupConfig config, const Executable& func) {
    Driver::Instance()->addGroup(move(config), func);
}

void setUp(Executable func) {
    Driver::Instance()->addSetUp(move(func));
}

void tearDown(Executable func) {
    Driver::Instance()->addTearDown(move(func));
}

void fail(const string& message) {
    throw ExpectationFailed(message);
}

}
