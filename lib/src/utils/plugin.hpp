#ifndef KKTEST_UTILS_PLUGIN_H_
#define KKTEST_UTILS_PLUGIN_H_

#include <map>
#include <string>
#include <vector>

namespace kktest {

class Plugin {
public:
    explicit Plugin(const std::string& group);

    virtual ~Plugin();

    virtual bool isEnabled() const;

    virtual void install();

    virtual void uninstall();

private:
    static std::map<std::string, std::vector<Plugin*>> plugins;

friend class Pluginable;
};

class Pluginable {
public:
    explicit Pluginable(const std::string& _pluginGroupName);

    virtual ~Pluginable();

    void installPlugins();

private:
    std::string pluginGroupName;
};

}

#endif
