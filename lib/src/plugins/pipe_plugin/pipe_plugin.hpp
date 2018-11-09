#ifndef KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_PLUGIN_H_
#define KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_PLUGIN_H_

#include <utils/plugin.hpp>
#include "pipe.hpp"


namespace kktest {

class PipePlugin: public Plugin {
public:
    bool isEnabled() const override;

    void install() override;

    void uninstall() override;

private:
    Pipe* pipe = nullptr;
};

}

#endif
