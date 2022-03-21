#include "ext.hpp"

#include <iostream>
#include <memory>

namespace mcga::test {

StdoutLoggingExtension::StdoutLoggingExtension(bool quiet,
                                               bool printSkipped,
                                               bool liveLogging) {
    if (!quiet) {
        logger = make_unique<TestLogger>(std::cout, liveLogging, printSkipped);
    }
}

void StdoutLoggingExtension::beforeTestExecution(const Test& test) {
    if (logger != nullptr) {
        logger->onTestExecutionStart(test);
    }
}

void StdoutLoggingExtension::afterTestExecution(const Test& test) {
    if (logger != nullptr) {
        logger->onTestExecutionFinish(test);
    }
}

void StdoutLoggingExtension::onWarning(const Warning& warning) {
    if (logger != nullptr) {
        logger->printWarning(warning);
    }
}

void StdoutLoggingExtension::destroy() {
    if (logger != nullptr) {
        logger->printFinalInformation();
    }
}

}  // namespace mcga::test
