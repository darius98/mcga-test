#ifndef KKTEST_UTILS_PLUGIN_H_
#define KKTEST_UTILS_PLUGIN_H_

#include <vector>

namespace kktest {

class Plugin {
public:
    virtual ~Plugin();

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
