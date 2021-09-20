#include "main.hpp"

#include <iostream>

#include "box_executor.hpp"
#include "driver.hpp"
#include "scan_executor.hpp"

using mcga::cli::ChoiceArgumentSpec;
using mcga::cli::NumericArgumentSpec;
using mcga::cli::Parser;

namespace mcga::test {

void runTestsOnExecutor(Executor* executor,
                        const std::vector<TestCase*>& tests) {
    Driver driver(executor);
    Driver::Init(&driver);
    for (TestCase* testCase: tests) {
        driver.addGroup(testCase->name, testCase->body);
    }
    Driver::Clean();
}

void runTests(int argc,
              char** argv,
              const std::vector<TestCase*>& tests,
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
          [] { return std::to_string(std::thread::hardware_concurrency()); },
          "Number of CPUs ("
            + std::to_string(std::thread::hardware_concurrency()) + ")"));

    for (Extension* extension: extensions) {
        extension->registerCommandLineArgs(&parser);
    }

    parser.parse(argc, argv);

    ExtensionApi api;
    for (Extension* extension: extensions) {
        extension->init(&api);
    }

    ScanExecutor scanner(&api);
    runTestsOnExecutor(&scanner, tests);

    std::unique_ptr<Executor> executor;
    switch (executorTypeArg->get_value()) {
        case Executor::SMOOTH: {
            executor = std::make_unique<Executor>(&api);
            break;
        }
        case Executor::BOXED: {
            executor = std::make_unique<BoxExecutor>(
              &api, std::max(maxParallelTestsArg->get_value(), 1ul));
            break;
        }
        default: {
            std::cout << "Unknown executor type. Aborting.";
            exit(1);
        }
    }
    runTestsOnExecutor(executor.get(), tests);
    for (Extension* extension: extensions) {
        extension->destroy();
    }
}

}  // namespace mcga::test
