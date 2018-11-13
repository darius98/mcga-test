#ifndef KKTEST_UTILS_PLUGIN_H_
#define KKTEST_UTILS_PLUGIN_H_

#include <map>
#include <string>
#include <vector>

namespace kktest {

class Plugin {
public:
    explicit Plugin(const std::string& _group);

    virtual ~Plugin();

    virtual bool isEnabled() const;

    virtual void install();

    virtual void uninstall();

private:
    std::string group;

    static Plugin** plugins;
    static int numPlugins;
    static int pluginsCapacity;

friend class Pluginable;
};

class Pluginable {
public:
    explicit Pluginable(const std::string& _pluginGroupName);

    virtual ~Pluginable();

    void installPlugins();

    void uninstallPlugins();

private:
    std::string pluginGroupName;
};

}

#endif
