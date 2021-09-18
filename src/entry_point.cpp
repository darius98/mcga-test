#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"

namespace mcga::test {

std::vector<TestCase*> testCasesRegistered;

MCGA_TEST_EXPORT
  TestCase::TestCase(Executable body, std::string name, Context context)
        : body(std::move(body)), name(std::move(name)), context(context) {
    testCasesRegistered.push_back(this);
}

}  // namespace mcga::test

MCGA_TEST_EXPORT int main(int argc, char** argv) {
    mcga::test::feedback::FeedbackExtension feedbackExtension;
    runTests(argc, argv, mcga::test::testCasesRegistered, {&feedbackExtension});
    return feedbackExtension.getReturnCode();
}
