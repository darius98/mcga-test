#include "main.hpp"

#include "driver.hpp"

namespace mcga::test {

TestCase* getTestCasesListHead();

void runTests(Executor* runner, const EntryPointOptions& options) {
    ExtensionApi api(options.extensions, options.numExtensions);
    Driver driver(&api, runner);
    api.init();
    for (int i = 0; i < options.numRuns; i++) {
        Driver::Init(&driver);
        for (TestCase* t = getTestCasesListHead(); t != nullptr; t = t->next) {
            driver.addGroup(GroupConfig{.description = t->name},
                            Executable(t->body, t->context));
        }
        Driver::Clean();
    }
    api.destroy();
}

}  // namespace mcga::test
