#define VERSION "0.0.2"

#include <filesystem>
#include <iostream>

#include <kktest_extension_api.hpp>
#include <explorer/explorer.hpp>

#include <test_execution_loop/test_execution_loop.hpp>

using namespace cppli;
using namespace kktest;
using namespace kktest::test_runner;
using namespace std;
using namespace std::filesystem;

int main(int argc, char** argv) {
    Cppli* cliApi = Cppli::create("KKTest Runner.");
    cliApi->addHelpFlag();

    Flag* versionFlag = cliApi->addFlag(FlagBuilder(
            "version",
            "Display program version.")
            .withShortName("v"));
    IntArgument* maxParallelCasesArgument = cliApi->addIntArgument(
            IntArgumentBuilder("parallel-cases",
                               "Maximum number of concurrent test cases.")
                               .withDefaultValue(1)
                               .withImplicitValue(3));
    IntArgument* maxParallelTestsPerCaseArgument = cliApi->addIntArgument(
            IntArgumentBuilder(
                "parallel-tests-per-case",
                "Maximum number of concurrent tests per test case")
                .withDefaultValue(1)
                .withImplicitValue(5));
    vector<string> positional = cliApi->interpret(argc, argv);
    cliApi->checkHelpFlag();
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
