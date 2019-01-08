#define VERSION "0.0.2"

#include <filesystem>
#include <iostream>

#include <kktest_extension_api.hpp>
#include <explorer/explorer.hpp>

#include <test_execution_loop/test_execution_loop.hpp>

using kktest::String;
using kktest::arguments::ArgumentsApi;
using kktest::arguments::Flag;
using kktest::arguments::FlagBuilder;
using kktest::arguments::IntArgument;
using kktest::arguments::IntArgumentBuilder;
using kktest::test_runner::explore;
using kktest::test_runner::TestExecutionLoop;
using std::cout;
using std::invalid_argument;
using std::stoi;
using std::vector;
using std::filesystem::path;

int main(int argc, char** argv) {
    ArgumentsApi* argumentsApi = ArgumentsApi::create("KKTest Runner.");
    argumentsApi->addHelpFlag();

    Flag* versionFlag = argumentsApi->addFlag(FlagBuilder(
            "version",
            "Display program version.")
            .withShortName("v"));
    IntArgument* maxParallelCasesArgument = argumentsApi->addIntArgument(
            IntArgumentBuilder("parallel-cases",
                               "Maximum number of concurrent test cases.")
                               .withDefaultValue(1)
                               .withImplicitValue(3));
    IntArgument* maxParallelTestsPerCaseArgument = argumentsApi->addIntArgument(
            IntArgumentBuilder(
                "parallel-tests-per-case",
                "Maximum number of concurrent tests per test case")
                .withDefaultValue(1)
                .withImplicitValue(5));
    vector<String> positional = argumentsApi->interpret(argc, argv);
    argumentsApi->checkHelpFlag();
    if (versionFlag->get()) {
        cout << "KKTest test test_runner version " << VERSION << "\n";
        return 0;
    }
    String rootPath = ".";
    if (!positional.empty() && !positional[0].empty()) {
        rootPath = positional[0];
    }

    int maxParallelTestCases = maxParallelCasesArgument->get();
    int maxParallelTestsPerCase = maxParallelTestsPerCaseArgument->get();
    auto executionLoop = new TestExecutionLoop(maxParallelTestCases);
    cout << "Searching for test cases...\n";
    explore(path(rootPath.str()),
            [executionLoop, maxParallelTestsPerCase](path testCase) {
        cout << "\tFound test case at " << testCase.c_str() << "\n";
        executionLoop->addToLoop(testCase.c_str(), maxParallelTestsPerCase);
    });
    if (executionLoop->isEmpty()) {
        cout << "No test cases found. Exiting.\n";
        return 0;
    }
    return executionLoop->join();
}
