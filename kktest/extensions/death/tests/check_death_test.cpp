#include <csignal>
#include <iostream>

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

TEST_CASE(checkDeath, "Death extension: check death") {
    test("exit code 0", [] {
        auto func = [] {
            exit(0);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getExitCode(), isZero);
    });

    test("exit code 1", [] {
        auto func = [] {
            exit(1);
        };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getExitCode(), isEqualTo(1));
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
