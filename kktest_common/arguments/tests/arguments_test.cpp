#include <cstdlib>

#include <vector>

#include <kktest.hpp>

#include <kktest_common/arguments.hpp>
#include <kktest_common/strutil.hpp>

using kktest::expect;
using kktest::group;
using kktest::String;
using kktest::test;
using kktest::arguments::ArgumentsApi;
using kktest::strutil::copyAsCString;
using std::malloc;
using std::size_t;
using std::initializer_list;

void interpretArgs(ArgumentsApi* api, const initializer_list<String>& args) {
    size_t cStyleArgsSize = (args.size() + 1) * sizeof(char*);
    char** cStyleArgs = static_cast<char**>(malloc(cStyleArgsSize));
    int index = 0;
    for (const String& arg: args) {
        cStyleArgs[index++] = copyAsCString(arg);
    }
    cStyleArgs[args.size()] = nullptr;
    api->interpret(static_cast<int>(args.size()), cStyleArgs);
    for (size_t i = 0; i < args.size(); ++i) {
        free(cStyleArgs[i]);
    }
    free(cStyleArgs);
}

void kkTestCase(Arguments) {
}
