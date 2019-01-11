#include <cstdlib>

#include <vector>

#include <kktest.hpp>

#include <kktest_common/arguments.hpp>
#include <kktest_common/string.hpp>
#include <kktest_ext/core_matchers.hpp>

using kktest::expect;
using kktest::group;
using kktest::String;
using kktest::setUp;
using kktest::tearDown;
using kktest::test;
using kktest::arguments::Argument;
using kktest::arguments::ArgumentBuilder;
using kktest::arguments::ArgumentsApi;
using kktest::arguments::Flag;
using kktest::arguments::FlagBuilder;
using kktest::arguments::IntArgument;
using kktest::arguments::IntArgumentBuilder;
using kktest::core_matchers::isEqualTo;
using kktest::core_matchers::isFalse;
using kktest::core_matchers::isTrue;
using kktest::core_matchers::throws;
using std::malloc;
using std::size_t;
using std::initializer_list;
using std::vector;

void kkTestCase(Arguments) {
    ArgumentsApi* api = nullptr;

    auto interpretArgs = [&](const initializer_list<String>& args) {
        size_t cStyleArgsSize = (args.size() + 2) * sizeof(char*);
        char** cStyleArgs = static_cast<char**>(malloc(cStyleArgsSize));
        cStyleArgs[0] = String("binaryName").copyAsCStr();
        int index = 1;
        for (const String& arg : args) {
            cStyleArgs[index++] = arg.copyAsCStr();
        }
        int numCArgs = static_cast<int>(args.size() + 1);
        cStyleArgs[numCArgs] = nullptr;
        auto positional = api->interpret(numCArgs, cStyleArgs);
        for (size_t i = 0; i < numCArgs; ++i) {
            free(cStyleArgs[i]);
        }
        free(cStyleArgs);
        return positional;
    };

    setUp([&] { api = ArgumentsApi::create("Help prefix."); });
    tearDown([&] { delete api; api = nullptr; });

    group("Single string argument", [&] {
        Argument* arg = nullptr;

        setUp([&] {
            arg = api->addArgument(ArgumentBuilder("name", "Description.")
                    .withShortName("n")
                    .withDefaultValue("a")
                    .withImplicitValue("b"));
        });

        test("no value provided leads to argument taking default value", [&] {
            interpretArgs({});
            expect(arg->get(), isEqualTo("a"));
        });

        test("provided with single dash, short name", [&] {
            interpretArgs({"-n"});
            expect(arg->get(), isEqualTo("b"));
        });

        test("provided with double dash, short name", [&] {
            interpretArgs({"--n"});
            expect(arg->get(), isEqualTo("b"));
        });

        test("provided with double dash, long name", [&] {
            interpretArgs({"--name"});
            expect(arg->get(), isEqualTo("b"));
        });

        test("value provided with single dash & space", [&] {
            interpretArgs({"-n", "v"});
            expect(arg->get(), isEqualTo("v"));
        });

        test("value provided with single dash & equal sign", [&] {
            interpretArgs({"-n=v"});
            expect(arg->get(), isEqualTo("v"));
        });

        test("value provided with double dash & space is positional, while "
             "the argument takes implicit value", [&] {
            auto positionalArgs = interpretArgs({"--name", "v"});
            expect(arg->get(), isEqualTo("b"));
            expect(positionalArgs, isEqualTo(vector<String>{"v"}));
        });

        test("value provided with double dash & equal sign (short name)", [&] {
            interpretArgs({"--n=v"});
            expect(arg->get(), isEqualTo("v"));
        });

        test("value provided with double dash & equal sign (long name)", [&] {
            interpretArgs({"--name=v"});
            expect(arg->get(), isEqualTo("v"));
        });

        test("providing value for different argument name does not influence"
             " interesting argument", [&] {
            interpretArgs({"-m", "v"});
            expect(arg->get(), isEqualTo("a"));
        });

        test("when providing multiple values for one argument, it takes the "
             "last one", [&] {
            interpretArgs({"-n", "v1", "-n", "--name=v2"});
            expect(arg->get(), isEqualTo("v2"));
        });
    });

    group("Multiple arguments", [&] {
        Argument* a = nullptr;
        Argument* b = nullptr;
        Argument* c = nullptr;

        setUp([&] {
            a = api->addArgument(ArgumentBuilder("arg_a", "Description a.")
                                 .withShortName("a")
                                 .withDefaultValue("default")
                                 .withImplicitValue("implicit"));
            b = api->addArgument(ArgumentBuilder("arg_b", "Description b.")
                                 .withShortName("b")
                                 .withDefaultValue("default")
                                 .withImplicitValue("implicit"));
            c = api->addArgument(ArgumentBuilder("arg_c", "Description c.")
                                 .withShortName("c")
                                 .withDefaultValue("default")
                                 .withImplicitValue("implicit"));
        });

        test("Providing values for multiple arguments via double dash", [&] {
            interpretArgs({"--arg_a=value", "--b"});
            expect(a->get(), isEqualTo("value"));
            expect(b->get(), isEqualTo("implicit"));
            expect(c->get(), isEqualTo("default"));
        });

        test("Providing values for multiple arguments with multiple single "
             "dash arguments", [&] {
            interpretArgs({"-a", "-b", "value"});
            expect(a->get(), isEqualTo("implicit"));
            expect(b->get(), isEqualTo("value"));
            expect(c->get(), isEqualTo("default"));
        });

        test("Providing implicit values for multiple arguments via a single"
             " dash argument", [&] {
            interpretArgs({"-ab"});
            expect(a->get(), isEqualTo("implicit"));
            expect(b->get(), isEqualTo("implicit"));
            expect(c->get(), isEqualTo("default"));
        });

        test("Providing values for multiple arguments via a single dash"
             "argument & space for non-implicit value of the last one", [&] {
            interpretArgs({"-abc", "value"});
            expect(a->get(), isEqualTo("implicit"));
            expect(b->get(), isEqualTo("implicit"));
            expect(c->get(), isEqualTo("value"));
        });

        test("Providing values for multiple arguments via a single dash"
             "argument & equal sign for non-implicit value of the last one",
             [&] {
            interpretArgs({"-abc=value"});
            expect(a->get(), isEqualTo("implicit"));
            expect(b->get(), isEqualTo("implicit"));
            expect(c->get(), isEqualTo("value"));
        });
    });

    group("flags", [&] {
        Flag* a = nullptr;
        Flag* b = nullptr;

        setUp([&] {
            a = api->addFlag(FlagBuilder("flag_a", "Description a.")
                             .withShortName("a"));
            b = api->addFlag(FlagBuilder("flag_b", "Description b.")
                             .withShortName("b"));
        });

        test("Default flag value is false", [&] {
            expect(a->get(), isFalse);
            expect(b->get(), isFalse);
        });

        test("Implicit flag value is true", [&] {
            interpretArgs({"--flag_a"});
            expect(a->get(), isTrue);
            expect(b->get(), isFalse);

            interpretArgs({"--a"});
            expect(a->get(), isTrue);
            expect(b->get(), isFalse);

            interpretArgs({"-ab", "-a"});
            expect(a->get(), isTrue);
            expect(b->get(), isTrue);
        });

        test("Passing a flag value '1', 'TRUE' or 'ENABLED' makes it true "
             "(case insensitive)", [&] {
            interpretArgs({"--flag_a=TRUE"});
            expect(a->get(), isTrue);

            interpretArgs({"--flag_a=tRuE"});
            expect(a->get(), isTrue);

            interpretArgs({"--flag_a=ENABLED"});
            expect(a->get(), isTrue);

            interpretArgs({"--flag_a=EnabLEd"});
            expect(a->get(), isTrue);

            interpretArgs({"--flag_a=1"});
            expect(a->get(), isTrue);
        });

        test("Passing a flag any other value makes it false", [&] {
            interpretArgs({"--flag_b=DISABLED"});
            expect(b->get(), isFalse);

            interpretArgs({"--flag_b=ANYTHING_ELSE"});
            expect(b->get(), isFalse);

            // take care with typo-s!
            interpretArgs({"--flag_b=ENABLD"});
            expect(b->get(), isFalse);
        });
    });

    group("integer arguments", [&] {
        IntArgument* arg = nullptr;

        setUp([&] {
            arg = api->addIntArgument(IntArgumentBuilder("name", "Description.")
                                      .withShortName("n")
                                      .withDefaultValue(0)
                                      .withImplicitValue(1));
        });

        test("Passing an integer argument an integer value works", [&] {
            interpretArgs({"--name=17"});
            expect(arg->get(), isEqualTo(17));

            interpretArgs({"--name=-7"});
            expect(arg->get(), isEqualTo(-7));

            interpretArgs({"-n", "1337"});
            expect(arg->get(), isEqualTo(1337));
        });

        test("Passing an integer argument a non-integer value throws", [&] {
            expect([&] { interpretArgs({"--name=invalid"}); }, throws);
        });
    });

    group("Invalid argument names", [&] {
        test("Registering an argument with the same name as an existing one "
             "throws", [&] {
            api->addArgument(ArgumentBuilder("name", "description 1"));
            expect([&] {
                api->addArgument(ArgumentBuilder("name", "description 2"));
            }, throws);
        });

        test("Registering an argument with the same name as an existing one's "
             "short name throws", [&] {
            api->addArgument(ArgumentBuilder("name", "description 1")
                             .withShortName("n"));
            expect([&] {
                api->addArgument(ArgumentBuilder("n", "description 2"));
            }, throws);
        });

        test("Registering an argument with the same short name as an existing "
             "one's name throws", [&] {
            api->addArgument(ArgumentBuilder("n", "description 1"));
            expect([&] {
                api->addArgument(ArgumentBuilder("name", "description 2")
                                 .withShortName("n"));
            }, throws);
        });

        test("Registering an argument with the same short name as an existing "
             "one's short name throws", [&] {
            api->addArgument(ArgumentBuilder("name", "description 1")
                             .withShortName("n"));
            expect([&] {
                api->addArgument(ArgumentBuilder("name2", "description 2")
                                 .withShortName("n"));
            }, throws);
        });

        test("Registering an argument with a short name that is longer than "
             "one character throws", [&] {
            expect([&] {
                api->addArgument(ArgumentBuilder("name", "description")
                                 .withShortName("nnn"));
            }, throws);
        });
    });
}
