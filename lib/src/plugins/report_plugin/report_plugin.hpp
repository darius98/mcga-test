#ifndef KKTEST_PLUGINS_REPORT_PLUGIN_REPORT_PLUGIN_H_
#define KKTEST_PLUGINS_REPORT_PLUGIN_REPORT_PLUGIN_H_

#include <core/plugin.hpp>


namespace kktest {

class ReportPlugin: public Plugin {
public:
    bool isEnabled() const override;

    void install() override;
};

}

#endif