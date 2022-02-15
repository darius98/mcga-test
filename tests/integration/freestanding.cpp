#include <mcga/test.hpp>

#include "core/main.hpp"
#include "extensions/exit_code/ext.hpp"

TEST_CASE("test case") {
    mcga::test::test("passing-test", [] {
        mcga::test::expect(1 + 1 == 2);
    });

    mcga::test::test("failing-test", [] {
        mcga::test::expect(1 + 1 == 3);
    });
}

int main() {
    mcga::test::ExitCodeExtension exitCodeExtension(false, false);
    auto extensions = makeExtensionArray(&exitCodeExtension);
    const auto options = mcga::test::EntryPointOptions{
      .extensions = extensions.data(),
      .numExtensions = extensions.size(),
      .numRuns = 1,
    };
    mcga::test::SmoothExecutor executor;
    runTests(&executor, options);
    return exitCodeExtension.getExitCode();
}

[[noreturn]] void start() asm("start");

void start() {
    mcga::test::ExitCodeExtension exitCodeExtension(false, false);
    auto extensions = makeExtensionArray(&exitCodeExtension);
    const auto options = mcga::test::EntryPointOptions{
      .extensions = extensions.data(),
      .numExtensions = extensions.size(),
      .numRuns = 1,
    };
    mcga::test::SmoothExecutor executor;
    runTests(&executor, options);
    exit(exitCodeExtension.getExitCode());
}

extern "C" void exit(int status) {
    
    __builtin_unreachable();
}
