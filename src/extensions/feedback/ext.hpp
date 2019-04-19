#pragma once

#include <mcga/proc.hpp>

#include "core/extension.hpp"
#include "test_logger.hpp"

namespace mcga::test::feedback {

class FeedbackExtension : public Extension {
  public:
    int getReturnCode();

    void registerCommandLineArgs(cli::Parser* parser) override;

    void init(HooksManager* api) override;

  private:
    static void addPipeHooks(proc::PipeWriter* pipe, HooksManager* api);

    void initLogging(HooksManager* api);

    void initFileStream(HooksManager* api, const std::string& fileName);

    void initSocketStream(HooksManager* api, const std::string& socketPath);

    int exitCode = 0;
    std::unique_ptr<TestLogger> logger = nullptr;
    std::unique_ptr<proc::PipeWriter> fileWriter = nullptr;
    std::unique_ptr<proc::PipeWriter> socketWriter = nullptr;

    cli::Flag quietFlag;
    cli::Argument fileNameArgument;
    cli::Argument socketPathArgument;
    cli::Flag noLiveLogging;
};

}  // namespace mcga::test::feedback
