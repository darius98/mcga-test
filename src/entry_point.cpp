#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"
#include "extensions/mock/ext.hpp"

namespace mcga::test::internal {

// Not exposed in any header.
std::vector<TestCase*> getTestCases();

}  // namespace mcga::test::internal

MCGA_TEST_EXPORT_WEAK int main(int argc, char** argv) {
    mcga::test::feedback::FeedbackExtension feedbackExtension;
    mcga::test::mock::MockExtension mockExtension;
    mcga::test::runTests(argc,
                         argv,
                         mcga::test::internal::getTestCases(),
                         {&feedbackExtension, &mockExtension});
    return feedbackExtension.getReturnCode();
}
