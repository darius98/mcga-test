#include <EasyFlags.hpp>

#include <iostream>

#include <explorer/explorer.hpp>

#define VERSION "0.0.1"

using namespace easyflags;
using namespace kktest_runner;
using namespace std;

AddArgument(int, flagVersion)
    .Name("version")
    .Short("v")
    .ImplicitValue(1)
    .DefaultValue(0);

int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    if (flagVersion) {
        cout << "KKTest test runner version " << VERSION << "\n";
        return 0;
    }
    vector<File> testCases = explore();
    if (testCases.empty()) {
        cout << "No test cases found.\n";
        return 0;
    }
    cout << "Discovered the following test cases:\n";
    for (const File& file: testCases) {
        cout << "\t" << file.toString() << "\n";
    }
    return 0;
}
