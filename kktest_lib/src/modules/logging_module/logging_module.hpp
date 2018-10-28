#ifndef KKTEST_MODULES_LOGGING_MODULE_LOGGING_MODULE_H_
#define KKTEST_MODULES_LOGGING_MODULE_LOGGING_MODULE_H_

#include <core/module.hpp>
#include "logger.hpp"


namespace kktest {

class LoggingModule: public Module {
public:
    LoggingModule();

    ~LoggingModule();

    bool isEnabled() const override;

    void install() override;

private:
    Logger* logger = nullptr;
};

}

#endif
