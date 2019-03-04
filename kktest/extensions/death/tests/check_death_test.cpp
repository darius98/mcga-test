#include <csignal>

#include <kktest.hpp>
#include <kktest_ext/matchers/comparison.hpp>
#include <kktest_ext/matchers/iterable.hpp>
#include <kktest_ext/matchers/numeric.hpp>
#include <kktest_ext/matchers/truth.hpp>

#include "kktest/extensions/death/include/kktest_ext/death_impl/check_death.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace kktest::death;
using namespace std;

void checkDeathTest() {
    test("exit code 0, no output", [] {
        auto func = [] {
            exit(0);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getOutput(), isEmpty);
        expect(status.getExitCode(), isZero);
    });

    test("exit code 1, no output", [] {
        auto func = [] {
            exit(1);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getOutput(), isEmpty);
        expect(status.getExitCode(), isEqualTo(1));
    });

    test("exit code 0, small output", [] {
        auto func = [] {
            cout << string(100, 'a');
            exit(0);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getOutput(), isEqualTo(string(100, 'a')));
        expect(status.getExitCode(), isEqualTo(0));
    });

    test("exit code 214, small output", [] {
        auto func = [] {
            cout << string(100, 'a');
            exit(214);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getOutput(), isEqualTo(string(100, 'a')));
        expect(status.getExitCode(), isEqualTo(214));
    });

    test("exit code 0, large output", [] {
        auto func = [] {
            cout << string(40000, 'a');
            exit(0);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getOutput(), isEqualTo(string(40000, 'a')));
        expect(status.getExitCode(), isEqualTo(0));
    });

    test("exit code 214, large output", [] {
        auto func = [] {
            cout << string(40000, 'a');
            exit(214);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getOutput(), isEqualTo(string(40000, 'a')));
        expect(status.getExitCode(), isEqualTo(214));
    });

    test("kill by signal SIGTERM", [] {
        auto func = [] {
            raise(SIGTERM);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isFalse);
        expect(status.killedBySignal(), isTrue);
        expect(status.getSignal(), isEqualTo(SIGTERM));
    });

    test("kill by signal SIGSEGV", [] {
        auto func = [] {
            raise(SIGSEGV);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isFalse);
        expect(status.killedBySignal(), isTrue);
        expect(status.getSignal(), isEqualTo(SIGSEGV));
    });
}
