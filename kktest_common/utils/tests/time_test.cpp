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
using kktest::utils::sleepForMs;
using kktest::utils::Stopwatch;
using kktest::utils::Timer;

void kkTestCase(UtilsTime) {
    group("Stopwatch::Starting a stopwatch with 4.99ms", [] {
        Stopwatch* watch = nullptr;
        setUp([&] { watch = new Stopwatch(4.99); });
        tearDown([&] { delete watch; watch = nullptr; });

        test("isElapsed() returns false when called immediately", [&] {
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test("isElapsed() returns false when called after 2ms", [&] {
            sleepForMs(2);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test("isElapsed() returns true when called after 5ms", [&] {
            sleepForMs(5);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });

        test("isElapsed() returns true when called after 7ms", [&] {
            sleepForMs(7);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });
    });

    group("Timer", [] {
        test("Calling getMsElapsed() immediately returns almost 0", [] {
            Timer t;
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 0.01));
        });

        test("Calling getMsElapsed() after 2ms returns almost 2", [] {
            Timer t;
            sleepForMs(2);
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
        });

        test("Calling getMsElapsed() several times returns correctly", [] {
            Timer t;
            sleepForMs(2);
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
            sleepForMs(2);
            expect(t.getMsElapsed(), isAlmostEqualTo(4, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(4, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(4, 0.01));
            sleepForMs(2);
            expect(t.getMsElapsed(), isAlmostEqualTo(6, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(6, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(6, 0.01));
        });
    });
}
