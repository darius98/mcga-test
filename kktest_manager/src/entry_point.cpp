#define VERSION "0.0.1"

#include <iostream>

#include <kktest_common/arguments.hpp>

using kktest::String;
using kktest::arguments::ArgumentsApi;
using kktest::arguments::Flag;
using kktest::arguments::FlagBuilder;
using std::cout;
using std::vector;

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
