#include <iostream>

#include <kktest_impl/main.hpp>
#include <kktest_impl/signature.hpp>
#include "driver.hpp"
#include "test_case_registry.hpp"

using namespace cppli;
using namespace std;

namespace kktest {

InternalArgs registerInternalFlags(Cppli* cliApi) {
    InternalArgs args;
    args.boxedFlag = cliApi->addFlag(
        FlagSpec("boxed")
        .setDescription("Run each test in an isolated process (boxed)")
        .setShortName("b"));
    args.maxParallelTestsArgument = cliApi->addNumericArgument(
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
