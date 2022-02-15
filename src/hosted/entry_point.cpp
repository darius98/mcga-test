#include "mcga/test.hpp"

#include <array>
#include <iostream>
#include <thread>

#include "mcga/cli.hpp"

#include "core/executor.hpp"
#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/binary_stream/ext.hpp"
#include "extensions/exit_code/ext.hpp"
#include "extensions/filter/ext.hpp"
#include "extensions/text_stream/ext.hpp"
#include "hosted/box_executor.hpp"

MCGA_TEST_EXPORT_WEAK int main(int argc, char** argv) {
    mcga::cli::Parser parser(
      "Test binary. Generated by The MCGA::Test Framework.");

    parser.add_help_flag();
    auto executorTypeArg = parser.add_choice_argument(
      mcga::cli::ChoiceArgumentSpec<mcga::test::Executor::Type>("executor")
        .set_description("Choose the type of executor to use. A smooth "
                         "executor runs all tests in the same process, "
                         "while a boxed executor runs each test in a "
                         "separate process. Using a smooth executor "
                         "means a test killed by signal will not be "
                         "detected and will kill the whole test suite.")
        .set_options({{"smooth", mcga::test::Executor::SMOOTH},
                      {"boxed", mcga::test::Executor::BOXED}})
        .set_default_value("boxed"));
    auto maxParallelTestsArg = parser.add_numeric_argument<size_t>(
      mcga::cli::NumericArgumentSpec("max-parallel-tests")
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
      mcga::cli::NumericArgumentSpec("repeat")
        .set_description("Repeat the tests multiple times.")
        .set_default_value("1"));

    auto skipIsFail = parser.add_flag(
      mcga::cli::FlagSpec("fail-on-skip")
        .set_help_group("Feedback")
        .set_description("Consider skipped tests as failed."));
    auto ignoreWarnings = parser.add_flag(
      mcga::cli::FlagSpec("ignore-warnings")
        .set_help_group("Feedback")
        .set_description(
          "When this flag is set, a test run where all tests pass, but "
          "warnings were generated will exit with code 0 instead of 1."));

    mcga::test::StdoutLoggingExtension loggingExtension;
    mcga::test::SocketStreamingExtension streamingExtension;
    mcga::test::FilterExtension filterExtension;

    loggingExtension.registerCommandLineArgs(&parser);
    streamingExtension.registerCommandLineArgs(&parser);
    filterExtension.registerCommandLineArgs(&parser);
    parser.parse(argc, argv);

    mcga::test::ExitCodeExtension exitCodeExtension(
      skipIsFail->get_value(), ignoreWarnings->get_value());

    auto extensions = makeExtensionArray(&loggingExtension,
                                         &streamingExtension,
                                         &exitCodeExtension,
                                         &filterExtension);

    const auto options = mcga::test::EntryPointOptions{
      .extensions = extensions.data(),
      .numExtensions = extensions.size(),
      .numRuns = testRepeatMultiplierArg->get_value(),
    };

    switch (executorTypeArg->get_value()) {
        case mcga::test::Executor::SMOOTH: {
            mcga::test::SmoothExecutor executor;
            runTests(&executor, options);
            break;
        }
        case mcga::test::Executor::BOXED: {
            auto numBoxes = std::max(maxParallelTestsArg->get_value(), 1ul);
            mcga::test::BoxExecutor executor(numBoxes);
            runTests(&executor, options);
            break;
        }
        default: {
            std::cout << "Unknown executor type. Aborting.";
            return EXIT_FAILURE;
        }
    }

    return exitCodeExtension.getExitCode();
}
