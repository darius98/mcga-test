#ifndef KKTEST_CORE_PLUGIN_H_
#define KKTEST_CORE_PLUGIN_H_

#include <vector>

namespace kktest {

class Plugin {
public:
    virtual bool isEnabled() const;

    virtual void install();

    virtual void uninstall();
};

class Pluginable {
public:
    Pluginable();

    ~Pluginable();

    void addPlugins(const std::vector<Plugin*>& newPlugins);

    void installPlugins();
private:
    std::vector<Plugin*> plugins;
};

}

#endif
