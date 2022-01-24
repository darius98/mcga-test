#pragma once

#include <mcga/cli.hpp>

#include "core/extension.hpp"
#include "test_logger.hpp"

namespace mcga::test {

class StdoutLoggingExtension {
  public:
    void registerCommandLineArgs(cli::Parser* parser);

    void init(ExtensionApi* api);

    void destroy();

  private:
    void initLogging(ExtensionApi* api);

    std::unique_ptr<TestLogger> logger = nullptr;

    cli::Flag quietFlag;
    cli::Flag printSkipped;
    cli::Flag noLiveLogging;
};

}  // namespace mcga::test
