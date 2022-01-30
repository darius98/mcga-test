#pragma once

#include "mcga/cli.hpp"

#include "core/extension.hpp"
#include "test_logger.hpp"

namespace mcga::test {

class StdoutLoggingExtension {
  public:
    void registerCommandLineArgs(cli::Parser* parser);

    void init();

    void beforeTestExecution(const Test& test);

    void afterTestExecution(const Test& test);

    void onWarning(const Warning& warning);

    void destroy();

  private:
    std::unique_ptr<TestLogger> logger = nullptr;

    cli::Flag quietFlag;
    cli::Flag printSkipped;
    cli::Flag noLiveLogging;
};

}  // namespace mcga::test
