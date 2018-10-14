#include <EasyFlags.hpp>

#include "driver.hpp"
#include "testing.hpp"

using namespace easyflags;
using namespace kktest;
using namespace std;

int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    TestingDriver::init(argv[0]);

    testCase();

    return TestingDriver::destroy();
}
