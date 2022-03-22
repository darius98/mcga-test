#include "mcga/test.hpp"

#include <array>
#include <iostream>
#include <thread>

#include "mcga/cli.hpp"

#include "core/executor.hpp"
#include "core/export.hpp"
#include "core/main.hpp"
#include "extensions/binary_stream.hpp"
#include "extensions/exit_code.hpp"
#include "extensions/filter/ext.hpp"
#include "extensions/text_stream/ext.hpp"
#include "hosted/box_executor.hpp"
#include "hosted/socket_streaming.hpp"

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
        .set_options({{"smooth", mcga::test::Executor::Type::SMOOTH},
                      {"boxed", mcga::test::Executor::Type::BOXED}})
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

    auto skipIsFailArg = parser.add_flag(
      mcga::cli::FlagSpec("fail-on-skip")
        .set_help_group("Feedback")
        .set_description("Consider skipped tests as failed."));
    auto ignoreWarningsArg = parser.add_flag(
      mcga::cli::FlagSpec("ignore-warnings")
        .set_help_group("Feedback")
        .set_description(
          "When this flag is set, a test run where all tests pass, but "
          "warnings were generated will exit with code 0 instead of 1."));

    auto quietFlag = parser.add_flag(
      mcga::cli::FlagSpec("quiet")
        .set_help_group("Feedback")
        .set_description("Disable STDOUT logging for this test run")
        .set_short_name("q"));
    auto printSkipped = parser.add_flag(
      mcga::cli::FlagSpec("print-skipped")
        .set_help_group("Feedback")
        .set_description("Print the information for skipped tests."));
    auto noLiveLogging = parser.add_flag(
      mcga::cli::FlagSpec("no-live-logging")
        .set_help_group("Feedback")
        .set_description("Disable logging volatile messages "
                         "(that are then removed with '\\r' modifiers."));

    auto streamToSockArg = parser.add_argument(
      mcga::cli::ArgumentSpec("stream-to-socket")
        .set_help_group("Feedback")
        .set_description("A UNIX socket with write access for piping the test "
                         "results as they become available.")
        .set_default_value(""));

    auto descriptionFilterArgument = parser.add_list_argument(
      mcga::cli::ListArgumentSpec{"filter"}
        .set_short_name("f")
        .set_help_group("Filtering")
        .set_description(
          "Filter tests by their full description "
          "(e.g. \"TestCaseName::group description::...::test description\")"
          "You can use the '*' character to match any number of characters. A "
          "test's description must contain a match, so writing "
          "\"--filter=*foo*\" is equivalent to writing \"--filter=foo\". If "
          "multiple filters are provided, at least one of them must match.")
        .set_default_value({}));
    auto descriptionExcludeArgument = parser.add_list_argument(
      mcga::cli::ListArgumentSpec{"exclude"}
        .set_short_name("e")
        .set_help_group("Filtering")
        .set_description("Same as --filter, except matching tests are skipped "
                         "instead of being executed.")
        .set_default_value({}));
    auto locationFilterArgument = parser.add_list_argument(
      mcga::cli::ListArgumentSpec{"filter-loc"}
        .set_help_group("Filtering")
        .set_description(
          "Filter tests by source code location. "
          "Expects the format --filter-loc=FILE or --filter-loc=FILE:LINE or "
          "--filter-loc=FILE:LINE:COLUMN, where the FILE will automatically "
          "match the suffix of the file. You can use the '*' character "
          "within "
          "the FILE portion to match any number of characters. You can also "
          "use the '*' character instead of LINE or COLUMN (e.g. "
          "--filter-loc=my_tests.cpp is equivalent to "
          "--filter-loc=*my_tests.cpp:*:*). If multiple filters are provided, "
          "at least one of them must match.")
        .set_default_value({}));
    auto locationExcludeArgument = parser.add_list_argument(
      mcga::cli::ListArgumentSpec{"exclude-loc"}
        .set_help_group("Filtering")
        .set_description(
          "Same as --filter-loc, except matching tests are "
          "skipped instead of being executed. Using '*' for LINE or COLUMN, or "
          "not including the :LINE or :LINE:COLUMN section means the "
          "line/column is not considered when filtering.")
        .set_default_value({}));

    parser.parse(argc, argv);

    mcga::test::StdoutLoggingExtension loggingExtension{
      quietFlag->get_value(),
      printSkipped->get_value(),
      !noLiveLogging->get_value()};
    auto streamingExtension = mcga::test::makeSocketStreamingExtension(
      streamToSockArg->get_value_if_exists());
    mcga::test::ExitCodeExtension exitCodeExtension(
      skipIsFailArg->get_value(), ignoreWarningsArg->get_value());
    mcga::test::FilterExtension filterExtension{
      descriptionFilterArgument->get_value(),
      descriptionExcludeArgument->get_value(),
      locationFilterArgument->get_value(),
      locationExcludeArgument->get_value(),
    };

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
        case mcga::test::Executor::Type::SMOOTH: {
            mcga::test::SmoothExecutor executor;
            runTests(&executor, options);
            break;
        }
        case mcga::test::Executor::Type::BOXED: {
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
