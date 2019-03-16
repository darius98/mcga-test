#include "kktest/core/include/kktest.hpp"

#include "kktest/core/driver.hpp"
#include "kktest/core/export.hpp"
#include "kktest/core/warning.hpp"

using namespace std;

namespace kktest {

KKTEST_EXPORT void test(TestConfig config, Executable body) {
    if (!Driver::IsInsideTestCase()) {
        EmitWarning("kktest::test() called outside any test case!\n",
                    "    Ignoring...");
        return;
    }
    Driver::Instance()->addTest(move(config), move(body));
}

KKTEST_EXPORT void group(GroupConfig config, const Executable& func) {
    if (!Driver::IsInsideTestCase()) {
        EmitWarning("kktest::group() called outside any test case!\n",
                    "    Ignoring...");
        return;
    }
    Driver::Instance()->addGroup(move(config), func);
}

KKTEST_EXPORT void setUp(Executable func) {
    if (!Driver::IsInsideTestCase()) {
        EmitWarning("kktest::setUp() called outside any test case!\n",
                    "    Ignoring...");
        return;
    }
    Driver::Instance()->addSetUp(move(func));
}

KKTEST_EXPORT void tearDown(Executable func) {
    if (!Driver::IsInsideTestCase()) {
        EmitWarning("kktest::tearDown() called outside any test case!\n",
                    "    Ignoring...");
        return;
    }
    Driver::Instance()->addTearDown(move(func));
}

KKTEST_EXPORT void fail(const string& message) {
    if (!Driver::IsInsideTestCase()) {
        EmitWarning("kktest::fail() called outside any test case!\n",
                    "    Ignoring...");
        return;
    }
    throw ExpectationFailed(message);
}

}
