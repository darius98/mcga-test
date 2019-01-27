#define VERSION "0.0.1"

#include <iostream>

#include <cppli.hpp>

using namespace cppli;
using namespace std;

int main(int argc, char** argv) {
    Cppli cppli("KKTest Manager.");
    cppli.addTerminalFlag(FlagSpec("help")
                          .setShortName("h")
                          .setDescription("Display this help menu."),
                          [&]() {
        cout << cppli.renderHelp();
    });
    cppli.addTerminalFlag(FlagSpec("version")
                          .setShortName("v")
                          .setDescription("Display program version."),
                          [&]() {
        cout << "KKTest Manager version " << VERSION << "\n";
    });
    vector<string> positional = cppli.interpret(argc, argv);
    return 0;
}
