#include <kktest.hpp>
#include <kktest_ext/matchers.hpp>

#include "common/utils/src/process_time.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace kktest::utils;

void kkTestCase(UtilsTime) {
    group("Duration", [] {
        group("Factory methods", [] {
            test("Duration::seconds(1) is 1000000000ns", [] {
                expect(Duration::seconds(1).totalNs(), isEqualTo(1000000000));
            });

            test("Duration::seconds(7) is 7000000000ns", [] {
                expect(Duration::seconds(7).totalNs(), isEqualTo(7000000000LL));
            });

            test("Duration::seconds(0.01) is 10000000ns", [] {
                expect(Duration::seconds(0.01).totalNs(), isEqualTo(10000000));
            });

            test("Duration::milliseconds(1) is 1000000ns", [] {
                expect(Duration::milliseconds(1).totalNs(), isEqualTo(1000000));
            });

            test("Duration::milliseconds(100) is 100000000ns", [] {
                expect(Duration::milliseconds(100).totalNs(),
                       isEqualTo(100000000));
            });

            test("Duration::milliseconds(.1) is 100000ns", [] {
                expect(Duration::milliseconds(.1).totalNs(), isEqualTo(100000));
            });

            test("Duration::nanoseconds(1) is 1ns", [] {
                expect(Duration::nanoseconds(1).totalNs(), isEqualTo(1));
            });

            test("Duration::nanoseconds(14000000000) is 14000000000ns", [] {
                expect(Duration::nanoseconds(14000000000LL).totalNs(),
                       isEqualTo(14000000000LL));
            });

        });

        group("Constructors, normalization", [] {
            test("Default constructor makes 0ns duration", [] {
                expect(Duration().totalNs(), isEqualTo(0));
                expect(Duration().totalNs(), isEqualTo(0));
                expect(Duration().totalNs(), isEqualTo(0));
                expect(Duration().totalNs(), isEqualTo(0));
            });

            test("Duration(1, 5) is 1000000005ns", [] {
                expect(Duration(1, 5LL).totalNs(), isEqualTo(1000000005));
            });

            test("Duration(1, 1000000000) is 2s + 0ns", [] {
                expect(Duration(1, 1000000000).getSeconds(), isEqualTo(2));
                expect(Duration(1, 1000000000).getNanoseconds(), isEqualTo(0));
                expect(Duration(1, 1000000000).totalNs(),
                       isEqualTo(2000000000));
            });

            test("Duration(1, 3000000005) is 4s + 5ns", [] {
                expect(Duration(1, 3000000005LL).getSeconds(), isEqualTo(4));
                expect(Duration(1, 3000000005LL).getNanoseconds(),
                       isEqualTo(5));
                expect(Duration(1, 3000000005LL).totalNs(),
                       isEqualTo(4000000005LL));
            });
        });

        test("Equality", [] {
            expect(Duration(1, 1), isEqualTo(Duration(1, 1)));

            Duration a(5, 100);
            expect(a, isEqualTo(a));
        });

        group("Addition", [] {
            test("Without overflow", [] {
                Duration a(1, 1);
                Duration b(2, 100);
                Duration c = a + b;
                expect(c.getSeconds(), isEqualTo(3));
                expect(c.getNanoseconds(), isEqualTo(101));
                expect(c.totalNs(), isEqualTo(3000000101LL));
                expect(c, isEqualTo(Duration(3, 101)));
            });

            test("With overflow", [] {
                Duration a(1, 900000009);
                Duration b(2, 100000000);
                Duration c = a + b;
                expect(c.getSeconds(), isEqualTo(4));
                expect(c.getNanoseconds(), isEqualTo(9));
                expect(c.totalNs(), isEqualTo(4000000009LL));
                expect(c, isEqualTo(Duration(4, 9)));
            });

            test("With negative", [] {
                Duration a(1, 0);
                Duration b(-1, -100);
                Duration c = a + b;
                expect(c.getSeconds(), isEqualTo(-1));
                expect(c.getNanoseconds(), isEqualTo(999999900));
                expect(c.totalNs(), isEqualTo(-100));
                expect(c, isEqualTo(Duration(0, -100)));
            });
        });

        group("Substraction", [] {
            test("Without overflow", [] {
                Duration a(1, 100);
                Duration b(1, 0);
                Duration c = a - b;
                expect(c.getSeconds(), isEqualTo(0));
                expect(c.getNanoseconds(), isEqualTo(100));
                expect(c.totalNs(), isEqualTo(100));
                expect(c, isEqualTo(Duration(0, 100)));
            });

            test("With overflow", [] {
                Duration a(1, 100);
                Duration b(1, 300);
                Duration c = a - b;
                expect(c.getSeconds(), isEqualTo(-1));
                expect(c.getNanoseconds(), isEqualTo(1000000000 - 200));
                expect(c.totalNs(), isEqualTo(-200));
                expect(c, isEqualTo(Duration(0, -200)));
            });

            test("With negative", [] {
                Duration a(1, 100);
                Duration b(-1, -300);
                Duration c = a - b;
                expect(c.getSeconds(), isEqualTo(2));
                expect(c.getNanoseconds(), isEqualTo(400));
                expect(c.totalNs(), isEqualTo(2000000400));
                expect(c, isEqualTo(Duration(2, 400)));
            });
        });
    });

    group("ProcessStopwatch::Starting a stopwatch with 50ms", [] {
        ProcessStopwatch* watch = nullptr;

        setUp([&] {
            watch = new ProcessStopwatch(Duration::milliseconds(50));
        });

        tearDown([&] {
            delete watch;
            watch = nullptr;
        });

        test("isElapsed() returns false when called immediately", [&] {
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test("isElapsed() returns false when called after 30ms", [&] {
            spinForDuration(Duration::milliseconds(30));
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
            expect(watch->isElapsed(), isFalse);
        });

        test("isElapsed() returns true when called after 51ms", [&] {
            spinForDuration(Duration::milliseconds(51));
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });

        test("isElapsed() returns true when called after 70ms", [&] {
            spinForDuration(Duration::milliseconds(70));
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
            expect(watch->isElapsed(), isTrue);
        });
    });

    group("ProcessTimer", [] {
        test("Calling getMsElapsed() immediately returns almost 0", [] {
            ProcessTimer t;
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(0, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(0, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(0, 0.1));
        });

        test("Calling getMsElapsed() after 20ms returns around 20", [] {
            ProcessTimer t;
            spinForDuration(Duration::milliseconds(20));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(20, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(20, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(20, 0.1));
        });

        test("Calling getMsElapsed() several times returns correctly", [] {
            ProcessTimer t;
            spinForDuration(Duration::milliseconds(20));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(20, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(20, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(20, 0.1));
            spinForDuration(Duration::milliseconds(20));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(40, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(40, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(40, 0.1));
            spinForDuration(Duration::milliseconds(20));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(60, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(60, 0.1));
            expect(1.0 * t.elapsed().totalNs() / Duration::kMilliToNano,
                   isAlmostEqualTo(60, 0.1));
        });
    });
}
