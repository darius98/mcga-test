#include "exit_code.hpp"

#include <cstdlib>

namespace mcga::test {

ExitCodeExtension::ExitCodeExtension(bool skipIsFail, bool ignoreWarnings)
        : exitCode(EXIT_SUCCESS), skipIsFail(skipIsFail),
          ignoreWarnings(ignoreWarnings) {
}

int ExitCodeExtension::getExitCode() const {
    return exitCode;
}

void ExitCodeExtension::afterTestExecution(const Test& test) {
    if (test.isFinished() && !test.isOptional()) {
        if (test.isFailed() || (test.isSkipped() && skipIsFail)) {
            exitCode = EXIT_FAILURE;
        }
    }
    if (test.isFinished() && test.isSkipped()) {
        skippedAnyTests = true;
    }
    if (test.isFinished() && test.isPassed()) {
        passedAnyTests = true;
    }
}

void ExitCodeExtension::onWarning(const Warning&) {
    if (!ignoreWarnings) {
        exitCode = EXIT_FAILURE;
    }
}

void ExitCodeExtension::destroy() {
    if (skippedAnyTests && !passedAnyTests) {
        exitCode = EXIT_FAILURE;
    }
}

}  // namespace mcga::test
