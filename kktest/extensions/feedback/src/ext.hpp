#pragma once

#include "common/interproc/src/pipe.hpp"
#include "kktest/core/src/extension.hpp"
#include "kktest/extensions/feedback/src/test_logger.hpp"

namespace kktest::feedback {

class FeedbackExtension: public Extension {
 public:
    void registerCommandLineArgs(cppli::Parser& parser) override;

    void init(HooksManager& api) override;

 private:
    void initLogging(HooksManager& api);

    void initPipe(HooksManager& api, const std::string& pipeName);

    std::unique_ptr<TestLogger> logger = nullptr;
    std::unique_ptr<interproc::PipeWriter> pipe = nullptr;

    cppli::Flag quietFlag;
    cppli::Argument pipeNameArgument;
};

}
