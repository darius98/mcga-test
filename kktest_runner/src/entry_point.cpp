#include <EasyFlags.hpp>

#include <iostream>

#include <explorer/explorer.hpp>
#include <test_execution_loop/test_execution_loop.hpp>

#define VERSION "0.0.2"

using namespace easyflags;
using namespace fsystem;
using namespace runner;
using namespace std;

AddArgument(int, flagVersion)
    .Name("version")
    .Short("v")
    .ImplicitValue(1)
    .DefaultValue(0);

AddArgument(int, argumentMaxParallelTestCases)
    .Name("parallel_cases")
    .ImplicitValue(5)
    .DefaultValue(1);

AddArgument(int, argumentParallelTestsPerCase)
    .Name("parallel_tests_per_case")
    .ImplicitValue(3)
    .DefaultValue(1);

int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    if (flagVersion) {
        cout << "KKTest test kktest_runner version " << VERSION << "\n";
        return 0;
    }
    auto executionLoop = new TestExecutionLoop(argumentMaxParallelTestCases);
    cout << "Searching for test cases...\n";
    explore([executionLoop](File testCase) {
        cout << "\tFound test case at " << testCase.toString() << "\n";
        executionLoop->addToLoop(testCase.toString(), argumentParallelTestsPerCase);
    });
    if (executionLoop->isEmpty()) {
        cout << "No test cases found. Exiting.\n";
        return 0;
    }
    return executionLoop->join();
}