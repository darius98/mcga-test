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

        test(testConfig(
                _.description = "isElapsed() returns false when called immediately",
                _.optional = true), [&] {
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test(testConfig(
                _.description = "isElapsed() returns false when called after 2ms",
                _.optional = true), [&] {
            sleepForMs(2);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test(testConfig(
                _.description = "isElapsed() returns true when called after 5ms",
                _.optional = true), [&] {
            sleepForMs(5);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });

        test(testConfig(
                _.description = "isElapsed() returns true when called after 7ms",
                _.optional = true), [&] {
            sleepForMs(7);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });
    });

    group("Timer", [] {
        test(testConfig(
                _.description = "Calling getMsElapsed() immediately returns almost 0",
                _.optional = true), [] {
            Timer t;
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(0, 0.01));
        });

        test(testConfig(
                _.description = "Calling getMsElapsed() after 2ms returns almost 2",
                _.optional = true), [] {
            Timer t;
            sleepForMs(2);
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
            expect(t.getMsElapsed(), isAlmostEqualTo(2, 0.01));
        });

        test(testConfig(
                _.description = "Calling getMsElapsed() several times returns correctly",
                _.optional = true), [] {
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
