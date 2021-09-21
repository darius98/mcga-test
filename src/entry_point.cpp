#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"

MCGA_TEST_EXPORT_WEAK int main(int argc, char** argv) {
    mcga::test::feedback::FeedbackExtension feedbackExtension;
    mcga::test::runTests(
      argc, argv, mcga::test::internal::getTestCases(), {&feedbackExtension});
    return feedbackExtension.getReturnCode();
}
