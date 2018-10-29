#ifndef KKTEST_CORE_MODULE_H_
#define KKTEST_CORE_MODULE_H_

namespace kktest {

class Module {
public:
    virtual bool isEnabled() const { return true; }

    virtual void install() {}

    virtual void uninstall() {}
};

}

#endif
