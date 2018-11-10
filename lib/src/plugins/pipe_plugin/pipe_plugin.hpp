#ifndef KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_PLUGIN_H_
#define KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_PLUGIN_H_

#include <utils/plugin.hpp>
#include "pipe.hpp"


namespace kktest {

extern Plugin* pipePlugin;

class PipePlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override;

    void install() override;

    void uninstall() override;

private:
    Pipe* pipe = nullptr;
};

}

#endif
