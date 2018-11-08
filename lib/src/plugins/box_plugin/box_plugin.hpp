#ifndef KKTEST_PLUGINS_BOX_PLUGIN_BOX_PLUGIN_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_BOX_PLUGIN_H_

#include <core/plugin.hpp>
#include "box_executor.hpp"


namespace kktest {

class BoxPlugin: public Plugin {
public:
    bool isEnabled() const override;

    void install() override;

    void uninstall() override;

private:
    BoxExecutor* boxExecutor;
};

}

#endif
