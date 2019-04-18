#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"

using std::vector;

namespace mcga::test {

vector<TestCase*> testCasesRegistered;

MCGA_TEST_EXPORT void TestCase::Register(TestCase* testCase) {
    testCasesRegistered.push_back(testCase);
}

}  // namespace mcga::test

MCGA_TEST_EXPORT int main(int argc, char** argv) {
    mcga::test::feedback::FeedbackExtension feedbackExtension;
    runTests(argc, argv, mcga::test::testCasesRegistered, {&feedbackExtension});
    return feedbackExtension.getReturnCode();
}
