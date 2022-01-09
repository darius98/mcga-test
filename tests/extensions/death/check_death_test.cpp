#include <csignal>
#include <iostream>

#include "mcga/test.hpp"
#include "mcga/test_ext/matchers.hpp"

#include "mcga/test_ext/death/check_death.hpp"

using namespace mcga::test;
using namespace mcga::matchers;
using namespace mcga::test::death;

static auto t = TestCase{"Death extension: check death"} + [] {
    test("exit code 0", [] {
        auto func = [] { exit(0); };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getExitCode(), isZero);
    });

    test("exit code 1", [] {
        auto func = [] { exit(1); };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isTrue);
        expect(status.killedBySignal(), isFalse);
        expect(status.getExitCode(), isEqualTo(1));
    });

    test("kill by signal SIGTERM", [] {
        auto func = [] { raise(SIGTERM); };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isFalse);
        expect(status.killedBySignal(), isTrue);
        expect(status.getSignal(), isEqualTo(SIGTERM));
    });

    test("kill by signal SIGUSR1", [] {
        auto func = [] { raise(SIGUSR1); };

        auto status = checkDeath(func);
        expect(status.exitedOrKilled(), isTrue);
        expect(status.exited(), isFalse);
        expect(status.killedBySignal(), isTrue);
        expect(status.getSignal(), isEqualTo(SIGUSR1));
    });
};
