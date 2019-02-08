#include <kktest_impl/main.hpp>
#include <kktest_impl/signature.hpp>
#include "driver.hpp"
#include "extension_api_impl.hpp"
#include "test_case_registry.hpp"

using namespace cppli;
using namespace std;

namespace kktest {

InternalArgs registerInternalFlags(Parser& parser, const string& versionString) {
    parser.addTerminalFlag(
            FlagSpec("version")
            .setShortName("v")
            .setDescription("Display program version."),
            "KKTest generated test-case.\n"
            "KKTest version: " + versionString +  "\n");
    parser.addTerminalFlag(
            FlagSpec("get-signature")
            .setDescription("Display the KKTest 32-byte "
                            "signature in hexadecimal format"),
            string(kkTestSignatureHex) + "\n");

    InternalArgs args;
    args.boxedFlag = parser.addFlag(
        FlagSpec("boxed")
        .setDescription("Run each test in an isolated process (boxed)")
        .setShortName("b"));
    args.maxParallelTestsArgument = parser.addNumericArgument(
        NumericArgumentSpec<int>("max-parallel-tests")
        .setDescription("Maximum number of tests to execute in parallel "
                        "(processes to spawn) when running boxed")
        .setDefaultValue(1)
        .setImplicitValue(1));
    return args;
}

int main(const vector<Extension*>& extensions, InternalArgs args) {
    ExtensionApiImpl apiImpl;
    for (Extension* extension : extensions) {
        extension->init(&apiImpl);
    }

    int maxParallelTests = args.maxParallelTestsArgument.get();
    if (maxParallelTests < 1) {
        maxParallelTests = 1;
    }

    Driver* driver = Driver::init(apiImpl.getHooks(),
                                  args.boxedFlag.get(),
                                  maxParallelTests);
    int ret = 1;
    try {
        for (const auto& testCase : TestCaseRegistry::all()) {
            driver->beforeTestCase(testCase.second);
            testCase.first();
            driver->afterTestCase();
        }
        ret = driver->destroy();
    } catch(const ConfigurationError& error) {
        driver->forceDestroy(error);
    } catch(const ExpectationFailed& error) {
        driver->forceDestroy(ConfigurationError(
                String("Expectation failed in global scope: ") + error.what()));
    } catch(const exception& error) {
        driver->forceDestroy(ConfigurationError(
                String("Exception thrown in global scope: ") + error.what()));
    } catch(...) {
        driver->forceDestroy(ConfigurationError(
                "Non-exception object thrown in global scope."));
    }

    for (Extension* extension : extensions) {
        extension->destroy();
    }
    return ret;
}

}
