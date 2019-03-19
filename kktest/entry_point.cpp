#include "kktest/core/include/kktest.hpp"

#include <iostream>

#include "kktest/core/export.hpp"
#include "kktest/core/main.hpp"
#include "kktest/extensions/feedback/ext.hpp"

using namespace kktest;
using namespace std;

namespace kktest {

vector<TestCase*> testCasesRegistered;

KKTEST_EXPORT void TestCase::Register(TestCase* testCase) {
    testCasesRegistered.push_back(testCase);
}

}

KKTEST_EXPORT int main(int argc, char** argv) {
    feedback::FeedbackExtension feedbackExtension;
    runTests(argc, argv, testCasesRegistered, {
        &feedbackExtension
    });
    return feedbackExtension.getReturnCode();
}
