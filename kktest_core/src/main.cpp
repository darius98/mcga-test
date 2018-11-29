#include <EasyFlags.hpp>

#include <kktest_impl/main.hpp>
#include <kktest_impl/signature.hpp>
#include "driver.hpp"
#include "test_case_registry.hpp"

using namespace easyflags;
using namespace std;

// This is here for external tools to be able to identify the executable as a kktest test suite.

AddArgument(int, argumentGetSignature)
    .Name("get_signature")
    .Description("Display the KKTest 32-byte signature in hexadecimal format")
    .ArgumentType("flag")
    .DefaultValue(0)
    .ImplicitValue(1);

namespace kktest {

int main(const vector<Extension*>& extensions) {
    if (argumentGetSignature) {
        for (int i = 0; i < kkTestSigSize; ++ i) {
            cout << "0123456789ABCDEF"[kkTestSignature[i] >> 4]
                 << "0123456789ABCDEF"[kkTestSignature[i] & 15];
        }
        cout << "\n";
        return 0;
    }

    ExtensionApiImpl apiImpl;
    for (Extension* extension: extensions) {
        extension->init(&apiImpl);
    }

    TestingDriver* driver = TestingDriver::init(apiImpl.getHooks());
    int ret = 1;
    try {
        for (TestCaseRegistry::TestCase testCase: TestCaseRegistry::all()) {
            driver->beforeTestCase();
            testCase();
            driver->afterTestCase();
        }
        ret = driver->destroy();
    } catch(const ConfigurationError& error) {
        driver->forceDestroy(error);
    } catch(const ExpectationFailed& error) {
        driver->forceDestroy(
            ConfigurationError(string("Expectation failed in global scope: ") + error.what())
        );
    } catch(const exception& error) {
        driver->forceDestroy(
            ConfigurationError(string("Exception thrown in global scope: ") + error.what())
        );
    } catch(...) {
        driver->forceDestroy(
            ConfigurationError("Non-exception object thrown in global scope.")
        );
    }

    for (Extension* extension: extensions) {
        extension->destroy();
    }
    return ret;
}

}
