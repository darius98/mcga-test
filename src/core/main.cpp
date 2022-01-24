#include "main.hpp"

#include "box_executor.hpp"
#include "driver.hpp"
#include "scan_executor.hpp"

namespace mcga::test {

TestCase* getTestCasesListHead();

static void run(Driver* driver) {
    for (TestCase* t = getTestCasesListHead(); t != nullptr; t = t->next) {
        driver->addGroup(GroupConfig{.description = t->name},
                         Executable(t->body, t->context));
    }
}

void runTests(Executor* runner, const EntryPointOptions& options) {
    ExtensionApi api;
    ScanExecutor scanner;
    scanner.setApi(&api);
    runner->setApi(&api);
    for (std::size_t i = 0; i < options.numExtensions; i++) {
        options.extensions[i].init(options.extensions[i].data, &api);
    }
    Driver driver;
    Driver::Init(&driver);
    for (int i = 0; i < options.numRuns; i++) {
        driver.setExecutor(&scanner);
        run(&driver);
        driver.setExecutor(runner);
        run(&driver);
        driver.setExecutor(nullptr);
    }
    Driver::Clean();
    for (std::size_t i = 0; i < options.numExtensions; i++) {
        options.extensions[i].destroy(options.extensions[i].data);
    }
}

}  // namespace mcga::test
