#ifndef KKTEST_EXT_FEEDBACK_IMPL_EXT_H_
#define KKTEST_EXT_FEEDBACK_IMPL_EXT_H_

#include <kktest_plugin_api.hpp>

namespace kktest {
namespace feedback {

class FeedbackExtensionManager: public ExtensionManager {
public:
    void init(const std::function<void(Plugin*)>& registerPlugin) override;

    void destroy() override;

private:
    Plugin* loggingPlugin = nullptr;
    Plugin* pipePlugin = nullptr;
};

}
}

#endif
