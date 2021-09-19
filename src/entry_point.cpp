#include "mcga/test.hpp"

#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/feedback/ext.hpp"

static mcga::test::TestCase* head = nullptr;

namespace mcga::test {

MCGA_TEST_EXPORT
TestCase::TestCase(void (*body)(),
                   const char* name,
                   const char* file,
                   uint32_t line) noexcept
        : body(body), name(name), file(file), line(line), next(head) {
    head = this;
}

}  // namespace mcga::test

MCGA_TEST_EXPORT int main(int argc, char** argv) {
    std::vector<mcga::test::TestCase*> testCasesRegistered;
    for (auto testCase = head; testCase != nullptr; testCase = testCase->next) {
        testCasesRegistered.push_back(testCase);
    }
    std::reverse(testCasesRegistered.begin(), testCasesRegistered.end());
    mcga::test::feedback::FeedbackExtension feedbackExtension;
    runTests(argc, argv, testCasesRegistered, {&feedbackExtension});
    return feedbackExtension.getReturnCode();
}
