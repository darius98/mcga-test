#ifndef KKTEST_EXT_FEEDBACK_IMPL_EXT_HPP_
#define KKTEST_EXT_FEEDBACK_IMPL_EXT_HPP_

#include <kktest_common/interproc.hpp>
#include <kktest_extension_api.hpp>
#include <kktest_ext/feedback_impl/test_logger.hpp>

namespace kktest {
namespace feedback {

class FeedbackExtension: public Extension {
 public:
    void registerCommandLineArgs(cppli::Cppli* cliApi) override;

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
