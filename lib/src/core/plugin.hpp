#ifndef KKTEST_CORE_PLUGIN_H_
#define KKTEST_CORE_PLUGIN_H_

#include <map>
#include <string>
#include <vector>

namespace kktest {

class Plugin {
public:
    Plugin();

    virtual ~Plugin() = default;

    virtual bool isEnabled() const {
        return true;
    }

    virtual void install() {}

    virtual void uninstall() {}

private:
    static Plugin** plugins;
    static int numPlugins;
    static int pluginsCapacity;

friend class TestingDriver;
};

}

#endif
