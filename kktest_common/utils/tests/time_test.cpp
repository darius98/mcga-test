#include <kktest.hpp>
#include <kktest_ext/core_matchers.hpp>

#include <kktest_common/time.hpp>

using kktest::expect;
using kktest::group;
using kktest::setUp;
using kktest::tearDown;
using kktest::test;
using kktest::core_matchers::isFalse;
using kktest::core_matchers::isTrue;
using kktest::core_matchers::isAlmostEqualTo;
using kktest::utils::spinForMs;
using kktest::utils::ProcessStopwatch;
using kktest::utils::ProcessTimer;

void kkTestCase(UtilsTime) {
    group("ProcessStopwatch::Starting a stopwatch with 50ms", [] {
        ProcessStopwatch* watch = nullptr;
        setUp([&] { watch = new ProcessStopwatch(50); });
        tearDown([&] { delete watch; watch = nullptr; });

        test("isElapsed() returns false when called immediately", [&] {
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test("isElapsed() returns false when called after 30ms", [&] {
            spinForMs(30);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test("isElapsed() returns true when called after 51ms", [&] {
            spinForMs(51);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });

        test("isElapsed() returns true when called after 70ms", [&] {
            spinForMs(70);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });
    });

    group("Timer", [] {
        test("Calling getMsElapsed() immediately returns almost 0", [] {
            ProcessTimer t;
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 4));
        });

        test("Calling getMsElapsed() after 20ms returns around 20", [] {
            ProcessTimer t;
            spinForMs(20);
            expect(t.getMsElapsed(), isAlmostEqualTo(20, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(20, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(20, 4));
        });

        test("Calling getMsElapsed() several times returns correctly", [] {
            ProcessTimer t;
            spinForMs(20);
            expect(t.getMsElapsed(), isAlmostEqualTo(20, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(20, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(20, 4));
            spinForMs(20);
            expect(t.getMsElapsed(), isAlmostEqualTo(40, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(40, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(40, 4));
            spinForMs(20);
            expect(t.getMsElapsed(), isAlmostEqualTo(60, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(60, 4));
            expect(t.getMsElapsed(), isAlmostEqualTo(60, 4));
        });
    });
}
