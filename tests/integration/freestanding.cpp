#include <mcga/test.hpp>

#include "core/main.hpp"
#include "extensions/feedback/exit_code_extension.hpp"

using namespace mcga::test;

TEST_CASE("test case") {
    test("passing-test", [] {
        expect(1 + 1 == 2);
    });

    test("failing-test", [] {
        expect(1 + 1 == 3);
    });
};

int main() {
    ExitCodeExtension exitCodeExtension(false, false);
    auto extensions = makeExtensionArray(&exitCodeExtension);
    const auto options = EntryPointOptions{
      .extensions = extensions.data(),
      .numExtensions = extensions.size(),
      .numRuns = 1,
    };
    SmoothExecutor executor;
    runTests(&executor, options);
    return exitCodeExtension.getExitCode();
}
