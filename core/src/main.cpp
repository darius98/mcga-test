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
    args.smoothFlag = parser.addFlag(
        FlagSpec("smooth")
        .setDescription("Run all tests within the same process"
                        " (useful for step-by-step debugging).")
        .setShortName("s"));
    args.maxParallelTestsArgument = parser.addNumericArgument(
        NumericArgumentSpec<size_t>("max-parallel-tests")
        .setDescription("Maximum number of tests to execute in parallel "
                        "(processes to spawn) when running boxed")
        .setDefaultValue(1u)
        .setImplicitValue(1u));
    return args;
}

int main(const vector<Extension*>& extensions, InternalArgs args) {
    ExtensionApiImpl apiImpl;
    for (Extension* extension : extensions) {
        extension->init(&apiImpl);
    }

    Driver* driver = Driver::init(
        apiImpl.getHooks(),
        args.smoothFlag.get(),
        max(args.maxParallelTestsArgument.get(), 1ul));
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
