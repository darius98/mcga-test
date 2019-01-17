#define VERSION "0.0.1"

#include <iostream>

#include <cppli.hpp>

using namespace cppli;
using namespace std;

int main(int argc, char** argv) {
    auto cliApi = Cppli::create("KKTest Manager.");
    cliApi->addHelpFlag();

    auto versionFlag = cliApi->addFlag(FlagBuilder("version",
                                                   "Display program version.")
                                                   .withShortName("v"));

    vector<string> positional = cliApi->interpret(argc, argv);
    cliApi->checkHelpFlag();

    if (versionFlag->get()) {
        cout << "KKTest Manager version " << VERSION << "\n";
    }

    delete cliApi;
    return 0;
}
