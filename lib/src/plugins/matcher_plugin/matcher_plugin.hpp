#ifndef KKTEST_PLUGINS_MATCHER_PLUGIN_MATCHER_PLUGIN_H_
#define KKTEST_PLUGINS_MATCHER_PLUGIN_MATCHER_PLUGIN_H_

#include <utils/plugin.hpp>

namespace kktest {

class MatcherPlugin: public Plugin {
public:
    void install() override;
};

}

#endif
