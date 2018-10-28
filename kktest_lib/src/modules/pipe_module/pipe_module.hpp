#ifndef KKTEST_MODULES_PIPE_MODULE_PIPE_MODULE_H_
#define KKTEST_MODULES_PIPE_MODULE_PIPE_MODULE_H_

#include <core/module.hpp>
#include "pipe.hpp"


namespace kktest {

class PipeModule: public Module {
public:
    PipeModule();

    ~PipeModule();

    bool isEnabled() const override;

    void install() override;

private:
    Pipe* pipe = nullptr;
};

}

#endif
