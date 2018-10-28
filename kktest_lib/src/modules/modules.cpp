#include "modules.hpp"

#include "report_module/report_module.hpp"
#include "logging_module/logging_module.hpp"
#include "pipe_module/pipe_module.hpp"
#include "matcher_module/matcher_module.hpp"

using namespace std;

namespace kktest {

vector<Module*> getAllModules() {
    return {
        new ReportModule(),
        new LoggingModule(),
        new PipeModule(),
        new MatcherModule(),
    };
}

}
