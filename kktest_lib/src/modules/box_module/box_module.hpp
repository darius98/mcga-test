#ifndef KKTEST_MODULES_BOX_MODULE_BOX_MODULE_H_
#define KKTEST_MODULES_BOX_MODULE_BOX_MODULE_H_

#include <core/module.hpp>
#include <core/executor.hpp>


namespace kktest {

class BoxModule: public Module {
public:
    bool isEnabled() const override;

    void install() override;
};

}

#endif
