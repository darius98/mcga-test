#ifndef KKTEST_PLUGINS_REPORT_PLUGIN_REPORT_PLUGIN_H_
#define KKTEST_PLUGINS_REPORT_PLUGIN_REPORT_PLUGIN_H_

#include <JSON>

#include <utils/plugin.hpp>


namespace kktest {

extern Plugin* reportPlugin;

class ReportPlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override;

    void install() override;

private:
    autojson::JSON report;
    std::map<int, autojson::JSON> groupJSONs;
};

}

#endif
