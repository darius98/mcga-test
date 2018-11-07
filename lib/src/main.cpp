#include <vector>

#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include <core/testcase.hpp>
#include <plugins/plugins.hpp>

using namespace easyflags;
using namespace std;

namespace kktest {

int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    TestingDriver::init(getAllPlugins());
    testCase();
    return TestingDriver::destroy();
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return kktest::main(argc, argv);
}
