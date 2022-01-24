#include "exit_code_extension.hpp"

namespace mcga::test {

ExitCodeExtension::ExitCodeExtension(bool skipIsFail, bool ignoreWarnings)
        : skipIsFail(skipIsFail), ignoreWarnings(ignoreWarnings) {
}

int ExitCodeExtension::getExitCode() const {
    return exitCode;
}

void ExitCodeExtension::init(ExtensionApi* api) {
    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>([this](const Test& test) {
        if (test.isFinished() && !test.isOptional()) {
            if (test.isFailed()
                || (test.isSkipped() && skipIsFail)) {
                exitCode = EXIT_FAILURE;
            }
        }
        if (test.isFinished() && test.isSkipped()) {
            skippedAnyTests = true;
        }
        if (test.isFinished() && test.isPassed()) {
            passedAnyTests = true;
        }
    });
    if (!ignoreWarnings) {
        api->addHook<ExtensionApi::ON_WARNING>([this](const Warning& warning) {
            exitCode = EXIT_FAILURE;
        });
    }
}

void ExitCodeExtension::destroy() {
    if (skippedAnyTests && !passedAnyTests) {
        exitCode = EXIT_FAILURE;
    }
}

}  // namespace mcga::test
