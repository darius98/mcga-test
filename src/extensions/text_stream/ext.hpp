#pragma once

#include "core/extension.hpp"
#include "test_logger.hpp"

namespace mcga::test {

class StdoutLoggingExtension {
  public:
    StdoutLoggingExtension(bool quiet, bool printSkipped, bool liveLogging);

    void beforeTestExecution(const Test& test);

    void afterTestExecution(const Test& test);

    void onWarning(const Warning& warning);

    void destroy();

  private:
    std::unique_ptr<TestLogger> logger = nullptr;
};

}  // namespace mcga::test
