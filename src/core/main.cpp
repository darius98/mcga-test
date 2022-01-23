#include "main.hpp"

#include <iostream>

#include "box_executor.hpp"
#include "driver.hpp"
#include "scan_executor.hpp"

using mcga::cli::ChoiceArgumentSpec;
using mcga::cli::NumericArgumentSpec;
using mcga::cli::Parser;

namespace mcga::test {

static void runExecutor(Driver* driver, TestCase* testCasesListHead) {
    for (TestCase* t = testCasesListHead; t != nullptr; t = t->next) {
        driver->addGroup(GroupConfig{.description = t->name},
                         Executable(t->body, t->context));
    }
}

static void runTestsOnExecutor(ExtensionApi* api,
                               Executor* runner,
                               TestCase* testCasesListHead,
                               const std::vector<Extension*>& extensions,
                               int testRepeatMultiplier) {
    for (Extension* extension: extensions) {
        extension->init(api);
    }

    ScanExecutor scanner(api);
    Driver driver;
    Driver::Init(&driver);
    for (int i = 0; i < testRepeatMultiplier; i++) {
        driver.setExecutor(&scanner);
        runExecutor(&driver, testCasesListHead);
        driver.setExecutor(runner);
        runExecutor(&driver, testCasesListHead);
        driver.setExecutor(nullptr);
    }
    Driver::Clean();
    for (Extension* extension: extensions) {
        extension->destroy();
    }
}

void runTests(int argc,
              char** argv,
              const std::vector<Extension*>& extensions) {
    Parser parser("Test binary. Generated by The MCGA::Test Framework.");

    parser.add_help_flag();
    auto executorTypeArg = parser.add_choice_argument(
      ChoiceArgumentSpec<Executor::Type>("executor")
        .set_description("Choose the type of executor to use. A smooth "
                         "executor runs all tests in the same process, "
                         "while a boxed executor runs each test in a "
                         "separate process. Using a smooth executor "
                         "means a test killed by signal will not be "
                         "detected and will kill the whole test suite.")
        .set_options({{"smooth", Executor::SMOOTH}, {"boxed", Executor::BOXED}})
        .set_default_value("boxed"));
    auto maxParallelTestsArg = parser.add_numeric_argument<size_t>(
      NumericArgumentSpec("max-parallel-tests")
        .set_description("Maximum number of tests to execute in parallel"
                         " (processes to spawn). Ignored if `executor` "
                         "type is 'smooth'.")
        .set_default_value("1")
        .set_implicit_value_generator(
          [] {
              return std::to_string(std::thread::hardware_concurrency());
          },
          "Number of CPUs ("
            + std::to_string(std::thread::hardware_concurrency()) + ")"));
    auto testRepeatMultiplierArg = parser.add_numeric_argument<int>(
      NumericArgumentSpec("repeat")
        .set_description("Repeat the tests multiple times.")
        .set_default_value("1"));

    for (Extension* extension: extensions) {
        extension->registerCommandLineArgs(&parser);
    }

    parser.parse(argc, argv);

    const auto testRepeatMultiplier = testRepeatMultiplierArg->get_value();

    ExtensionApi api;

    switch (executorTypeArg->get_value()) {
        case Executor::SMOOTH: {
            SmoothExecutor executor(&api);
            runTestsOnExecutor(&api,
                               &executor,
                               getTestCasesListHead(),
                               extensions,
                               testRepeatMultiplier);
            break;
        }
        case Executor::BOXED: {
            auto numBoxes = std::max(maxParallelTestsArg->get_value(), 1ul);
            BoxExecutor executor(&api, numBoxes);
            runTestsOnExecutor(&api,
                               &executor,
                               getTestCasesListHead(),
                               extensions,
                               testRepeatMultiplier);
            break;
        }
        default: {
            std::cout << "Unknown executor type. Aborting.";
            exit(1);
        }
    }
}

}  // namespace mcga::test
