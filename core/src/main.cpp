#include "core/src/main.hpp"

#include "core/src/driver.hpp"
#include "core/src/extension_api.hpp"
#include "core/src/test_case_registry.hpp"

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
    ExtensionApi api;
    for (Extension* extension : extensions) {
        extension->init(&api);
    }

    Driver* driver = Driver::init(
        api.getHooks(),
        args.smoothFlag.get(),
        max(args.maxParallelTestsArgument.get(), 1ul));
    int ret = 1;
    try {
        for (const auto& testCase : TestCaseRegistry::all()) {
            driver->beforeTestCase(testCase.second);
            testCase.first();
            driver->afterTestCase();
        }
        ret = driver->clean();
    } catch(const ConfigurationError& error) {
        driver->forceDestroy(error);
    } catch(const ExpectationFailed& error) {
        driver->forceDestroy(ConfigurationError(
                string("Expectation failed in global scope: ") + error.what()));
    } catch(const exception& error) {
        driver->forceDestroy(ConfigurationError(
                string("Exception thrown in global scope: ") + error.what()));
    } catch(...) {
        driver->forceDestroy(ConfigurationError(
                "Non-exception object thrown in global scope."));
    }

    for (Extension* extension : extensions) {
        extension->destroy();
    }
    delete driver;
    return ret;
}

}
