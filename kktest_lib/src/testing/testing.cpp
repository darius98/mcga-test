#include <EasyFlags.hpp>

#include "driver.hpp"
#include "testing.hpp"

namespace kktest {

int testMain(int argc, char** argv) {
    easyflags::ParseEasyFlags(argc, argv);
    TestingDriver::init(argv[0]);
    for (auto hook: TestingDriver::getInstance()->afterInitHooks) {
        hook();
    }

    testCase();

    return TestingDriver::destroy();
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return kktest::testMain(argc, argv);
}
