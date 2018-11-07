#ifndef KKTEST_PLUGINS_BOX_PLUGIN_BOX_PLUGIN_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_BOX_PLUGIN_H_

#include <core/plugin.hpp>
#include <core/executor.hpp>


namespace kktest {

class BoxPlugin: public Plugin {
public:
    bool isEnabled() const override;

    void install() override;
};

}

#endif
