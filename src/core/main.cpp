#include "main.hpp"

#include "driver.hpp"
#include "test_case.hpp"

namespace mcga::test {

TestCaseList* getTestCasesList();

void runTests(Executor* runner, const EntryPointOptions& options) {
    ExtensionApi api(options.extensions, options.numExtensions);
    Driver driver(&api, runner);
    api.init();
    for (int i = 0; i < options.numRuns; i++) {
        Driver::Init(&driver);
        for (const TestCase& t: *getTestCasesList()) {
            driver.addGroup(GroupConfig{.description = t.name},
                            Executable(t.body, t.context));
        }
        Driver::Clean();
    }
    api.destroy();
}

}  // namespace mcga::test
