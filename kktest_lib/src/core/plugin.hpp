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
    explicit Pluginable(const std::vector<Plugin*>& _plugins);

    virtual ~Pluginable();

    void installPlugins();
private:
    std::vector<Plugin*> plugins;
};

}

#endif
