#ifndef KKTEST_EXT_FEEDBACK_IMPL_LOGGING_PLUGIN_H_
#define KKTEST_EXT_FEEDBACK_IMPL_LOGGING_PLUGIN_H_

#include <kktest_ext/feedback_impl/test_logger.hpp>
#include <kktest_plugin_api.hpp>

namespace kktest {
namespace feedback {

class LoggingPlugin: public Plugin {
public:
    void install() override;

    void uninstall() override;

private:
    TestLogger* logger = nullptr;
};

}
}

#endif

