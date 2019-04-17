#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"

using namespace mcga::test;
using namespace std;

namespace mcga::test {

vector<TestCase*> testCasesRegistered;

MCGA_TEST_EXPORT void TestCase::Register(TestCase* testCase) {
    testCasesRegistered.push_back(testCase);
}

}  // namespace mcga::test

MCGA_TEST_EXPORT int main(int argc, char** argv) {
    feedback::FeedbackExtension feedbackExtension;
    runTests(argc, argv, testCasesRegistered, {&feedbackExtension});
    return feedbackExtension.getReturnCode();
}
