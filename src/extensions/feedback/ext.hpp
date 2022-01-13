#pragma once

#include <mcga/proc.hpp>

#include "core/extension.hpp"
#include "test_logger.hpp"

namespace mcga::test {

class FeedbackExtension : public Extension {
  public:
    void registerCommandLineArgs(cli::Parser* parser) override;

    void init(ExtensionApi* api) override;

  private:
    static void addPipeHooks(proc::PipeWriter* pipe, ExtensionApi* api);

    void initLogging(ExtensionApi* api);

    void initSocketStream(ExtensionApi* api, const std::string& socketPath);

    std::unique_ptr<TestLogger> logger = nullptr;
    std::unique_ptr<proc::PipeWriter> socketWriter = nullptr;

    cli::Flag quietFlag;
    cli::Flag printSkipped;
    cli::Argument socketPathArgument;
    cli::Flag noLiveLogging;
};

class ExitCodeExtension : public Extension {
  public:
    int getExitCode() const;

    void registerCommandLineArgs(cli::Parser* parser) override;

    void init(ExtensionApi* api) override;

  private:
    int exitCode = 0;
    bool skippedAnyTests = false;
    bool passedAnyTests = false;

    cli::Flag skipIsFail;
};

}  // namespace mcga::test
