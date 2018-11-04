#include <EasyFlags.hpp>

#include <iostream>

#define VERSION "0.0.1"

using namespace easyflags;
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
    return 0;
}
