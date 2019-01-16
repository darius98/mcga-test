#define VERSION "0.0.1"

#include <iostream>

#include <kktest_common/arguments.hpp>

using namespace kktest;
using namespace kktest::arguments;
using namespace std;

int main(int argc, char** argv) {
    auto argumentsApi = ArgumentsApi::create("KKTest Manager.");
    argumentsApi->addHelpFlag();

    auto versionFlag = argumentsApi->addFlag(
        FlagBuilder("version",
                    "Display program version.")
                    .withShortName("v"));

    vector<String> positional = argumentsApi->interpret(argc, argv);
    argumentsApi->checkHelpFlag();

    if (versionFlag->get()) {
        cout << "KKTest Manager version " << VERSION << "\n";
    }

    delete argumentsApi;
    return 0;
}
