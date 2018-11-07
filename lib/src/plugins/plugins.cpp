#include "plugins.hpp"

#include "box_plugin/box_plugin.hpp"
#include "report_plugin/report_plugin.hpp"
#include "logging_plugin/logging_plugin.hpp"
#include "pipe_plugin/pipe_plugin.hpp"
#include "matcher_plugin/matcher_plugin.hpp"

using namespace std;

namespace kktest {

vector<Plugin*> getAllPlugins() {
    return {
        new BoxPlugin(),
        new ReportPlugin(),
        new LoggingPlugin(),
        new PipePlugin(),
        new MatcherPlugin(),
    };
}

}
