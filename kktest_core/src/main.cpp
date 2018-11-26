#include <EasyFlags.hpp>

#include "driver.hpp"
#include "test_case_registry.hpp"

using namespace easyflags;
using namespace std;

namespace kktest {

int main() {
    TestingDriver::init();
    try {
        for (TestCaseRegistry::TestCase testCase: TestCaseRegistry::all()) {
            TestingDriver::beforeTestCase();
            testCase();
            TestingDriver::afterTestCase();
        }
    } catch(const ConfigurationError& error) {
        TestingDriver::forceDestroy(error);
        return 1;
    } catch(const ExpectationFailed& error) {
        TestingDriver::forceDestroy(
            ConfigurationError(string("Expectation failed in global scope: ") + error.what())
        );
        return 1;
    } catch(const exception& error) {
        TestingDriver::forceDestroy(
            ConfigurationError(string("Exception thrown in global scope: ") + error.what())
        );
        return 1;
    } catch(...) {
        TestingDriver::forceDestroy(
            ConfigurationError("Non-exception object thrown in global scope.")
        );
        return 1;
    }
    return TestingDriver::destroy();
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    return kktest::main();
}
