#include "main.hpp"

#include "hosted/box_executor.hpp"
#include "driver.hpp"

namespace mcga::test {

TestCase* getTestCasesListHead();

static void run(Driver* driver) {
    Driver::Init(driver);
    for (TestCase* t = getTestCasesListHead(); t != nullptr; t = t->next) {
        driver->addGroup(GroupConfig{.description = t->name},
                         Executable(t->body, t->context));
    }
    Driver::Clean();
}

void runTests(Executor* runner, const EntryPointOptions& options) {
    ExtensionApi api(options.extensions, options.numExtensions);
    runner->setExtensionApi(&api);
    for (std::size_t i = 0; i < options.numExtensions; i++) {
        options.extensions[i].init(options.extensions[i].data);
    }
    Driver driver(runner);
    for (int i = 0; i < options.numRuns; i++) {
        run(&driver);
    }
    for (std::size_t i = 0; i < options.numExtensions; i++) {
        options.extensions[i].destroy(options.extensions[i].data);
    }
}

}  // namespace mcga::test
