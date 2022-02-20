#include "main.hpp"

#include "driver.hpp"

namespace mcga::test {

namespace internal {

TestCase* getTestCasesHead();

}

void runTests(Executor* runner, const EntryPointOptions& options) {
    ExtensionApi api(options.extensions, options.numExtensions);
    Driver driver(&api, runner);
    api.init();
    for (int i = 0; i < options.numRuns; i++) {
        Driver::Init(&driver);
        for (auto testCase = internal::getTestCasesHead(); testCase != nullptr;
             testCase = testCase->next) {
            driver.addGroup(GroupConfig{.description = testCase->name},
                            Executable(testCase->body, testCase->context));
        }
        Driver::Clean();
    }
    api.destroy();
}

}  // namespace mcga::test
