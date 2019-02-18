#ifndef KKTEST_EXTENSIONS_FEEDBACK_SRC_EXT_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_SRC_EXT_HPP_

#include "common/interproc/src/pipe.hpp"
#include "core/src/extension_api.hpp"
#include "extensions/feedback/src/test_logger.hpp"

namespace kktest {
namespace feedback {

class FeedbackExtension: public Extension {
 public:
    void registerCommandLineArgs(cppli::Parser& parser) override;

    void init(ExtensionApi* api) override;

 private:
    void initLogging(ExtensionApi* api);

    void initPipe(ExtensionApi* api, const std::string& pipeName);

    std::unique_ptr<TestLogger> logger = nullptr;
    std::unique_ptr<interproc::PipeWriter> pipe = nullptr;

    cppli::Flag quietFlag;
    cppli::Argument pipeNameArgument;
};

}
}

#endif
