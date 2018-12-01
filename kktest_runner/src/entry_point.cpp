#include <iostream>

#include <kktest_extension_api>

#include <explorer/explorer.hpp>
#include <test_execution_loop/test_execution_loop.hpp>

#define VERSION "0.0.2"

using kktest::ArgumentsApi;
using kktest::Argument;
using kktest::Flag;
using fsystem::File;
using std::cout;
using std::invalid_argument;
using std::string;
using std::stoi;
using std::vector;
using runner::explore;
using runner::TestExecutionLoop;

int main(int argc, char** argv) {
    ArgumentsApi* argumentsApi = ArgumentsApi::create("KKTest Runner.");

    Flag* versionFlag = argumentsApi->addFlag(
            "version",
            "Display program version.",
            "v"
    );
    Argument* maxParallelCasesArgument = argumentsApi->addArgument(
            "parallel-cases",
            "Maximum number of concurrent test cases.",
            "",
            "1",
            "3"
    );
    Argument* maxParallelTestsPerCaseArgument = argumentsApi->addArgument(
            "parallel-tests-per-case",
            "Maximum number of concurrent tests per test case",
            "",
            "1",
            "5"
    );
    vector<string> positional = argumentsApi->interpret(argc, argv);
    if (versionFlag->get()) {
        cout << "KKTest test kktest_runner version " << VERSION << "\n";
        return 0;
    }
    string rootPath = ".";
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
    explore(rootPath, [executionLoop, maxParallelTestsPerCase](File testCase) {
        cout << "\tFound test case at " << testCase.toString() << "\n";
        executionLoop->addToLoop(testCase.toString(), maxParallelTestsPerCase);
    });
    if (executionLoop->isEmpty()) {
        cout << "No test cases found. Exiting.\n";
        return 0;
    }
    return executionLoop->join();
}
