#ifndef KKTEST_MODULES_REPORT_MODULE_H_
#define KKTEST_MODULES_REPORT_MODULE_H_

#include <testing/module.hpp>


namespace kktest {

class ReportModule: public Module {
public:
    bool isEnabled() const override;

    void install() override;
};

}

#endif
