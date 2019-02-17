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

    void destroy() override;

 private:
    void initLogging(ExtensionApi* api);

    void initPipe(ExtensionApi* api, const String& pipeName);

    TestLogger* logger = nullptr;
    interproc::PipeWriter* pipe = nullptr;

    cppli::Flag quietFlag;
    cppli::Argument pipeNameArgument;
};

}
}

#endif
