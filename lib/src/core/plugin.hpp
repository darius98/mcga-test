#ifndef KKTEST_CORE_PLUGIN_H_
#define KKTEST_CORE_PLUGIN_H_

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
    static std::vector<Plugin*>* plugins;

    static void clean();

friend class TestingDriver;
};

}

#endif
