#ifndef KKTEST_CORE_MODULE_H_
#define KKTEST_CORE_MODULE_H_

#include <vector>

namespace kktest {

// TODO: Rename to Plugin.
class Module {
public:
    virtual bool isEnabled() const;

    virtual void install();

    virtual void uninstall();
};

class Pluginable {
public:
    Pluginable();

    ~Pluginable();

    void addModules(const std::vector<Module*>& newModules);

    void installModules();
    void uninstallModules();
private:
    std::vector<Module*> modules;
};

}

#endif
