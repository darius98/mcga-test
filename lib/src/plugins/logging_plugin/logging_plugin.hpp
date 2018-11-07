#ifndef KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGING_PLUGIN_H_
#define KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGING_PLUGIN_H_

#include <core/plugin.hpp>
#include "logger.hpp"


namespace kktest {

class LoggingPlugin: public Plugin {
public:
    bool isEnabled() const override;

    void install() override;

    void uninstall() override;

private:
    Logger* logger = nullptr;
};

}

#endif
