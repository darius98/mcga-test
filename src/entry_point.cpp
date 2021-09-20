#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"

MCGA_TEST_EXPORT_WEAK int main(int argc, char** argv) {
    mcga::test::feedback::FeedbackExtension feedbackExtension;
    runTests(argc, argv, mcga::test::getTestCases(), {&feedbackExtension});
    return feedbackExtension.getReturnCode();
}
