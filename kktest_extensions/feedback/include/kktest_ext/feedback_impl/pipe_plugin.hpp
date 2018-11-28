#ifndef KKTEST_EXT_FEEDBACK_IMPL_PIPE_PLUGIN_H_
#define KKTEST_EXT_FEEDBACK_IMPL_PIPE_PLUGIN_H_

#include <messaging>

#include <kktest_plugin_api.hpp>

namespace kktest {
namespace feedback {

class PipePlugin: public Plugin {
public:
    explicit PipePlugin(const std::string& _pipeName);

    void install() override;

    void uninstall() override;

private:
    std::string pipeName;
    messaging::OutputPipe* pipe = nullptr;
};

}
}

#endif

