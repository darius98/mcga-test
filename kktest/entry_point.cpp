#include "kktest/core/include/kktest.hpp"

#include "kktest/core/export.hpp"
#include "kktest/core/main.hpp"
#include "kktest/extensions/feedback/ext.hpp"

using namespace mcga::test;
using namespace std;

namespace mcga::test {

vector<TestCase*> testCasesRegistered;

KKTEST_EXPORT void TestCase::Register(TestCase* testCase) {
    testCasesRegistered.push_back(testCase);
}

}  // namespace mcga::test

KKTEST_EXPORT int main(int argc, char** argv) {
    feedback::FeedbackExtension feedbackExtension;
    runTests(argc, argv, testCasesRegistered, {&feedbackExtension});
    return feedbackExtension.getReturnCode();
}
