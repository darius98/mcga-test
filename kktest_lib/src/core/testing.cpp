#include <vector>

#include <EasyFlags.hpp>

#include <plugins/plugins.hpp>
#include "driver.hpp"
#include "testing.hpp"

using namespace std;

namespace kktest {

int main(int argc, char** argv) {
    easyflags::ParseEasyFlags(argc, argv);
    TestingDriver* driver = TestingDriver::init(argv[0], getAllPlugins());

    for (Executable hook: TestingDriver::getInstance()->afterInitHooks) {
        hook();
    }
    testCase();
    int retCode = TestingDriver::destroy();
    return retCode;
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return kktest::main(argc, argv);
}
