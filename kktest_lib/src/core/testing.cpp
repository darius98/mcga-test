#include <vector>

#include <EasyFlags.hpp>

#include <modules/report_module/report_module.hpp>
#include <modules/logging_module/logging_module.hpp>
#include "driver.hpp"
#include "testing.hpp"

using namespace std;

namespace kktest {

int testMain(int argc, char** argv) {
    easyflags::ParseEasyFlags(argc, argv);
    TestingDriver::init(argv[0]);

    //TODO: Make a generic configureModules method/class/unit.
    vector<Module*> modules = {
        new ReportModule(),
        new LoggingModule()
    };
    for (Module* module: modules) {
        if (module->isEnabled()) {
            module->install();
        }
    }

    for (auto hook: TestingDriver::getInstance()->afterInitHooks) {
        hook();
    }
    testCase();
    int retCode = TestingDriver::destroy();

    for (Module* module: modules) {
        delete module;
    }

    return retCode;
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return kktest::testMain(argc, argv);
}
