#ifndef KKTEST_EXTENSIONS_FEEDBACK_INCLUDE_KKTEST_EXT_FEEDBACK_IMPL_EXT_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_INCLUDE_KKTEST_EXT_FEEDBACK_IMPL_EXT_HPP_

#include <messaging.hpp>

#include <kktest_extension_api.hpp>
#include <kktest_ext/feedback_impl/test_logger.hpp>

namespace kktest {
namespace feedback {

class FeedbackExtension: public Extension {
 public:
    void registerCommandLineArguments(ArgumentsApi* argumentsApi) override;

    void init(ExtensionApi* api) override;

    void destroy() override;

 private:
    void initLogging(ExtensionApi* api);

    void initPipe(ExtensionApi* api, const String& pipeName);

    TestLogger* logger = nullptr;
    messaging::OutputPipe* pipe = nullptr;

    Flag* quietFlag;
    Argument* pipeNameArgument;
};

}  // namespace feedback
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_FEEDBACK_INCLUDE_KKTEST_EXT_FEEDBACK_IMPL_EXT_HPP_
