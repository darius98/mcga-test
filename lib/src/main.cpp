#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include <core/testcase.hpp>

using namespace easyflags;
using namespace std;

namespace kktest {

int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    TestingDriver::init();
    kkTestCase();
    return TestingDriver::destroy();
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return kktest::main(argc, argv);
}
