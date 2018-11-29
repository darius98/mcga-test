#ifndef KKTEST_EXT_FEEDBACK_IMPL_EXT_H_
#define KKTEST_EXT_FEEDBACK_IMPL_EXT_H_

#include <messaging>

#include <kktest_extension_api>
#include <kktest_ext/feedback_impl/test_logger.hpp>

namespace kktest {
namespace feedback {

class FeedbackExtension: public Extension {
public:
    void init(ExtensionApi* api) override;

    void destroy() override;

private:
    void initLogging(ExtensionApi* api);

    void initPipe(ExtensionApi* api, const std::string& pipeName);

    TestLogger* logger = nullptr;
    messaging::OutputPipe* pipe = nullptr;
};

}
}

#endif
