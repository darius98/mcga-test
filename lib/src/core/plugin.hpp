#ifndef KKTEST_CORE_PLUGIN_H_
#define KKTEST_CORE_PLUGIN_H_

#include <map>
#include <string>
#include <vector>

namespace kktest {

class Plugin {
private:
    static Plugin** plugins;
    static int numPlugins;
    static int pluginsCapacity;

public:
    Plugin();

    virtual ~Plugin();

    virtual bool isEnabled() const;

    virtual void install();

    virtual void uninstall();

friend class TestingDriver;
};

}

#endif
