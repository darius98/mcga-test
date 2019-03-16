#include "kktest/core/include/kktest.hpp"

#include <iostream>

#include "kktest/core/src/export.hpp"
#include "kktest/core/src/main.hpp"
#include "kktest/extensions/feedback/src/ext.hpp"

using namespace kktest;
using namespace std;

namespace kktest {

vector<TestCase*> testCasesRegistered;

KKTEST_EXPORT void TestCase::Register(TestCase* testCase) {
    testCasesRegistered.push_back(testCase);
}

}

KKTEST_EXPORT int main(int argc, char** argv) {
    return kktest::runTests(argc, argv, testCasesRegistered, {
        new kktest::feedback::FeedbackExtension()
    });
}
