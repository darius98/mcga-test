#include <vector>

#include <EasyFlags.hpp>

#include <modules/modules.hpp>
#include "driver.hpp"
#include "testing.hpp"

using namespace std;

namespace kktest {

int main(int argc, char** argv) {
    easyflags::ParseEasyFlags(argc, argv);
    TestingDriver::init(argv[0]);

    vector<Module*> modules = getAllModules();

    for (Module* module: modules) {
        if (module->isEnabled()) {
            module->install();
        }
    }

    for (Executable hook: TestingDriver::getInstance()->afterInitHooks) {
        hook();
    }
    testCase();
    int retCode = TestingDriver::destroy();


    for (Module* module: modules) {
        if (module->isEnabled()) {
            module->uninstall();
        }
        delete module;
    }

    return retCode;
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return kktest::main(argc, argv);
}
