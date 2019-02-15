#ifndef KKTEST_EXT_FEEDBACK_IMPL_EXT_HPP_
#define KKTEST_EXT_FEEDBACK_IMPL_EXT_HPP_

#include <extension_api/extension_api.hpp>
#include <kktest_common/interproc.hpp>
#include <kktest_ext/feedback_impl/test_logger.hpp>

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
