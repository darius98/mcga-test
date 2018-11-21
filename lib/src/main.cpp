#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include <core/testcase.hpp>

using namespace easyflags;
using namespace std;

namespace kktest {

int main() {
    TestingDriver::init();
    try {
        kkTestCase();
    } catch(const ConfigurationError& error) {
        TestingDriver::forceDestroy(error);
        return 1;
    } catch(const ExpectationFailed& error) {
        TestingDriver::forceDestroy(
            ConfigurationError(string("Expectation failed in global scope: ") + error.what())
        );
    } catch(const exception& error) {
        TestingDriver::forceDestroy(
            ConfigurationError(string("Exception thrown in global scope: ") + error.what())
        );
    } catch(...) {
        TestingDriver::forceDestroy(
            ConfigurationError("Non-exception object thrown in global scope.")
        );
    }
    return TestingDriver::destroy();
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    return kktest::main();
}
