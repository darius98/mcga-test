#include <EasyFlags.hpp>

#include "driver.hpp"
#include "test_case_registry.hpp"

using namespace easyflags;
using namespace std;

// This is here for external tools to be able to identify the executable as a kktest test suite.
constexpr const int kkTestSigSize = 32;
const unsigned char kkTestSignature[] =
        "\x43\x00\xaa\x4f\x56\x6e\x0c\x64\xeb\xa1\xf5\x1d\x7c\xaa\xbc\xe8"
        "\xbf\x03\x2d\x86\x40\x69\x98\x65\xa3\x79\x51\xb4\x8a\x33\xce\x97";

AddArgument(int, argumentGetSignature)
    .Name("get_signature")
    .Description("Display the KKTest 32-byte signature in hexadecimal format")
    .ArgumentType("flag")
    .DefaultValue(0)
    .ImplicitValue(1);

namespace kktest {

int main() {
    if (argumentGetSignature) {
        for (int i = 0; i < kkTestSigSize; ++ i) {
            cout << "0123456789ABCDEF"[kkTestSignature[i] >> 4]
                 << "0123456789ABCDEF"[kkTestSignature[i] & 15];
        }
        cout << "\n";
        return 0;
    }

    TestingDriver::init();
    try {
        for (TestCaseRegistry::TestCase testCase: TestCaseRegistry::all()) {
            TestingDriver::beforeTestCase();
            testCase();
            TestingDriver::afterTestCase();
        }
        return TestingDriver::destroy();
    } catch(const ConfigurationError& error) {
        TestingDriver::forceDestroy(error);
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
    return 1;
}

}
