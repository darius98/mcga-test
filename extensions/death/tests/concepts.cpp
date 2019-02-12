#include <iostream>

#include <kktest.hpp>
#include <kktest_ext/core_matchers.hpp>
#include <kktest_ext/death.hpp>

using namespace kktest;
using namespace kktest::core_matchers;
using namespace kktest::death;
using namespace std;

void kkTestCase(ExitConcepts) {
    test("Simple exiting lambda (all cases must compile)", [] {
        auto exitingLambda = [] {
            exit(0);
        };

        expect(exitingLambda, exits);
        expect(exitingLambda, exits.withCode(0));
        expect(exitingLambda, exits.withCode(isZero));
        expect(exitingLambda, exits.withOutput(""));
        expect(exitingLambda, exits.withOutput(string("")));
        expect(exitingLambda, exits.withOutput(isEmpty));
        expect(exitingLambda, exits.withCode(0).withOutput(""));
        expect(exitingLambda, exits.withCode(0).withOutput(string("")));
        expect(exitingLambda, exits.withCode(isZero).withOutput(""));
        expect(exitingLambda, exits.withCode(isZero).withOutput(string("")));
        expect(exitingLambda, exits.withCode(0).withOutput(isEmpty));
        expect(exitingLambda, exits.withCode(isZero).withOutput(isEmpty));
        expect(exitingLambda, exits.withOutput("").withCode(0));
        expect(exitingLambda, exits.withOutput(string("")).withCode(0));
        expect(exitingLambda, exits.withOutput("").withCode(isZero));
        expect(exitingLambda, exits.withOutput(string("")).withCode(isZero));
        expect(exitingLambda, exits.withOutput(isEmpty).withCode(0));
        expect(exitingLambda, exits.withOutput(isEmpty).withCode(isZero));

        auto deathStatus = checkDeath(exitingLambda);
        expect(deathStatus, hasExited);
        expect(deathStatus, hasExitedWithCode(0));
        expect(deathStatus, hasExitedWithCode(isZero));
        expect(deathStatus, hasExitedWithCodeZero);
        expect(deathStatus, isNot(hasExitedWithNonZeroCode));
        expect(deathStatus, hasOutput(isEmpty));
        expect(deathStatus, hasOutput(""));
        expect(deathStatus, hasOutput(string("")));
    });

    test("Non-zero exit code", [] {
        expect([] {
            exit(17);
        }, exits.withCode(17));
    });

    test("Non-empty output", [] {
        expect([] {
            cout << "Hello, World!";
            exit(0);
        }, exits.withOutput("Hello, World!").withCode(0));
    });

    test("Non-empty output + non-zero exit code", [] {
        expect([] {
            cout << "Hello, World!";
            exit(13);
        }, exits.withOutput("Hello, World!").withCode(13));
    });
}
