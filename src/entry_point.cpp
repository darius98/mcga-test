#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"
#include "extensions/filter/ext.hpp"

MCGA_TEST_EXPORT_WEAK int main(int argc, char** argv) {
    mcga::test::FeedbackExtension feedbackExtension;
    mcga::test::ExitCodeExtension exitCodeExtension;
    mcga::test::FilterExtension filterExtension;
    mcga::test::runTests(argc,
                         argv,
                         mcga::test::getTestCases(),
                         {
                           &feedbackExtension,
                           &exitCodeExtension,
                           &filterExtension,
                         });
    return exitCodeExtension.getExitCode();
}
