#define VERSION "0.0.2"

#include <filesystem>
#include <iostream>

#include <kktest_extension_api.hpp>
#include <explorer/explorer.hpp>

#include <test_execution_loop/test_execution_loop.hpp>

using kktest::ArgumentsApi;
using kktest::Argument;
using kktest::Flag;
using kktest::String;
using kktest::test_runner::explore;
using kktest::test_runner::TestExecutionLoop;
using std::cout;
using std::invalid_argument;
using std::stoi;
using std::vector;
using std::filesystem::path;

int main(int argc, char** argv) {
    ArgumentsApi* argumentsApi = ArgumentsApi::create("KKTest Runner.");

    Flag* versionFlag = argumentsApi->addFlag(
            "version",
            "Display program version.",
            "v");
    Argument* maxParallelCasesArgument = argumentsApi->addArgument(
            "parallel-cases",
            "Maximum number of concurrent test cases.",
            "",
            "1",
            "3");
    Argument* maxParallelTestsPerCaseArgument = argumentsApi->addArgument(
            "parallel-tests-per-case",
            "Maximum number of concurrent tests per test case",
            "",
            "1",
            "5");
    vector<String> positional = argumentsApi->interpret(argc, argv);
    if (versionFlag->get()) {
        cout << "KKTest test test_runner version " << VERSION << "\n";
        return 0;
    }
    String rootPath = ".";
    if (!positional.empty() && !positional[0].empty()) {
        rootPath = positional[0];
    }

    int maxParallelTestCases;
    try {
        maxParallelTestCases = stoi(maxParallelCasesArgument->get());
    } catch(const invalid_argument& exc) {
        cout << "Invalid value provided for argument \"parallel-cases\": "
             << "\"" << maxParallelCasesArgument->get() << "\"\n";
        return 1;
    }
    int maxParallelTestsPerCase;
    try {
        maxParallelTestsPerCase = stoi(maxParallelTestsPerCaseArgument->get());
    } catch(const invalid_argument& exc) {
        cout << "Invalid value provided for argument \"parallel-tests-per-case\": "
             << "\"" << maxParallelCasesArgument->get() << "\"\n";
        return 1;
    }


    auto executionLoop = new TestExecutionLoop(maxParallelTestCases);
    cout << "Searching for test cases...\n";
    explore(path(rootPath), [executionLoop, maxParallelTestsPerCase](path testCase) {
        cout << "\tFound test case at " << testCase.c_str() << "\n";
        executionLoop->addToLoop(testCase.c_str(), maxParallelTestsPerCase);
    });
    if (executionLoop->isEmpty()) {
        cout << "No test cases found. Exiting.\n";
        return 0;
    }
    return executionLoop->join();
}