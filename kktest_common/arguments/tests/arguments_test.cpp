#include <cstdlib>

#include <vector>

#include <kktest.hpp>

#include <kktest_common/arguments.hpp>
#include <kktest_common/strutil.hpp>
#include <kktest_ext/core_matchers.hpp>

using kktest::expect;
using kktest::group;
using kktest::String;
using kktest::setUp;
using kktest::tearDown;
using kktest::test;
using kktest::arguments::Argument;
using kktest::arguments::ArgumentsApi;
using kktest::core_matchers::isEqualTo;
using kktest::strutil::copyAsCString;
using std::malloc;
using std::size_t;
using std::initializer_list;
using std::vector;

vector<String> interpretArgs(ArgumentsApi* api,
                             const initializer_list<String>& args) {
    size_t cStyleArgsSize = (args.size() + 2) * sizeof(char*);
    char** cStyleArgs = static_cast<char**>(malloc(cStyleArgsSize));
    cStyleArgs[0] = copyAsCString("binaryName");
    int index = 1;
    for (const String& arg: args) {
        cStyleArgs[index++] = copyAsCString(arg);
    }
    int numCArgs = static_cast<int>(args.size() + 1);
    cStyleArgs[numCArgs] = nullptr;
    auto positional = api->interpret(numCArgs, cStyleArgs);
    for (size_t i = 0; i < numCArgs; ++i) {
        free(cStyleArgs[i]);
    }
    free(cStyleArgs);
    return positional;
}

void kkTestCase(Arguments) {
    ArgumentsApi* api = nullptr;

    setUp([&api] { api = ArgumentsApi::create("Help prefix."); });
    tearDown([&api] { delete api; api = nullptr; });

    group("Single argument", [&api] {
        group("String argument", [&api] {
            Argument* arg = nullptr;

            setUp([&api, &arg] {
                arg = api->addArgument("name", "Description.", "n", "a", "b");
            });

            test("no value provided leads to argument taking default value",
                 [&api, &arg] {
                interpretArgs(api, {});
                expect(arg->get(), isEqualTo("a"));
            });

            test("provided with single dash, short name", [&api, &arg] {
                interpretArgs(api, {"-n"});
                expect(arg->get(), isEqualTo("b"));
            });

            test("provided with double dash, short name", [&api, &arg] {
                interpretArgs(api, {"--n"});
                expect(arg->get(), isEqualTo("b"));
            });

            test("provided with double dash, long name", [&api, &arg] {
                interpretArgs(api, {"--name"});
                expect(arg->get(), isEqualTo("b"));
            });

            test("value provided with single dash & space", [&api, &arg] {
                interpretArgs(api, {"-n", "v"});
                expect(arg->get(), isEqualTo("v"));
            });

            test("value provided with single dash & equal sign", [&api, &arg] {
                interpretArgs(api, {"-n=v"});
                expect(arg->get(), isEqualTo("v"));
            });

            test("value provided with double dash & space is positional, while "
                 "the argument takes implicit value", [&api, &arg] {
                auto positionalArgs = interpretArgs(api, {"--name", "v"});
                expect(arg->get(), isEqualTo("b"));
                expect(positionalArgs, isEqualTo(vector<String>{"v"}));
            });

            test("value provided with double dash & equal sign (short name)",
                 [&api, &arg] {
                interpretArgs(api, {"--n=v"});
                expect(arg->get(), isEqualTo("v"));
            });

            test("value provided with double dash & equal sign (long name)",
                 [&api, &arg] {
                interpretArgs(api, {"--name=v"});
                expect(arg->get(), isEqualTo("v"));
            });

            test("providing value for different argument name does not influence"
                 " interesting argument", [&api, &arg] {
                interpretArgs(api, {"-m", "v"});
                expect(arg->get(), isEqualTo("a"));
            });

            test("providing consecutive values to the same argument "
                 "considers only the last one", [&api, &arg] {
                interpretArgs(api, {"-n", "v1", "-n", "--name=v2"});
                expect(arg->get(), isEqualTo("v2"));
            });
        });
    });
}
