#include <EasyFlags.hpp>

#include "driver.hpp"
#include "testing.hpp"

using namespace easyflags;
using namespace kktest;
using namespace std;

int testMain(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    TestingDriver::init(argv[0]);

    testCase();

    return TestingDriver::destroy();
}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return testMain(argc, argv);
}
