#ifndef KKTEST_PLUGINS_MATCHER_PLUGIN_MATCHER_PLUGIN_H_
#define KKTEST_PLUGINS_MATCHER_PLUGIN_MATCHER_PLUGIN_H_

#include <utils/plugin.hpp>

namespace kktest {

extern Plugin* matcherPlugin;

class MatcherPlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override;

    void install() override;

private:
    int testsRunningCounter = 0;
};

}

#endif
