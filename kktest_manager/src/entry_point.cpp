#define VERSION "0.0.1"

#include <iostream>

#include <kktest_impl/arguments_api.hpp>

using kktest::ArgumentsApi;
using kktest::Flag;
using kktest::String;
using std::cout;
using std::vector;

int main(int argc, char** argv) {
    auto argumentsApi = ArgumentsApi::create("KKTest Manager.");

    auto versionFlag = argumentsApi->addFlag("version", "Display program version.", "v");

    vector<String> positional = argumentsApi->interpret(argc, argv);

    if (versionFlag->get()) {
        cout << "KKTest Manager version " << VERSION << "\n";
    }

    delete argumentsApi;
    return 0;
}
