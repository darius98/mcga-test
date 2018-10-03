/**
 * This file provides a conceptual usage of available matchers / tools in the
 * library. It is meant to fail some tests (so you can see the descriptions you
 * get when tests fail): actually, each passing test has a corresponding failing
 * test.
 *
 * Tests marked with (P) are intended to PASS, while tests marked with (F) are
 * intended to fail, to show you the error message.
 */

#include <vector>

#include <ktest>

using namespace ktest;
using namespace std;


int main() {
    vector<int> v, w;

    setUp([&]() {
        /**
         * This lambda expression is called before every test.
         *
         * We want each test to start with a couple of clean vectors.
         * To avoid writing bug-prone boiler-plate at the start of each test,
         * write the initialization in a setUp instead.
         */
        v = vector<int>();
        w = vector<int>();
    });

    test("Vector is initially empty - 0 (P)", [&]() {
        /**
         * This is the main way to assert conditions during tests.
         * expect(BOOLEAN_EXPRESSION) will fail and stop the test if the
         * expression does not evaluate to true.
         */
        expect(v.empty());
        /**
         * This is the exact equivalent for verifying a boolean condition, using
         * the `isTrue` matcher.
         */
        expectMatches(v.empty(), isTrue);
        /**
         * This is equivalent to the above, but with a nicer error message in
         * case of failure (see versions 1 and 2 of this test).
         */
        expectMatches(v, isEmpty);
    });

    test("Vector is initially empty - 1 (F)", [&]() {
        // When the vector is not empty, the isEmpty expectation will fail.
        v = {1, 2, 3};

        /**
         * This is the non-graceful failure - where no extra information is
         * available except the boolean expression that was evaluated and
         * failed.
         * This should display something similar to
         * `v.empty() is false`
         */
        expect(v.empty()); // Fails!
    });

    test("Vector is initially empty - 2 (F)", [&]() {
        // When the vector is not empty, the isEmpty expectation will fail.
        v = {1, 2, 3};

        /**
         * This is the graceful failure - where all information is available:
         * what was expected, what object was provided and WHY it failed.
         * This should display something similar to
         * `Expected empty iterable. Got '[1, 2, 3]': non-empty iterable`
         */
        expectMatches(v, isEmpty); // Fails!
    });

    test("Two empty vectors are equal (P)", [&]() {
         expectMatches(v, isEqualTo(w));
    });

    test("Two empty vectors are equal (F)", [&]() {
        w = {1};
        expectMatches(v, isEqualTo(w));
    });

    test("Two vectors are identical (P)", [&]() {
        expectMatches(v, isIdenticalTo(v));
    });

    test("Two vectors are identical (F)", [&]() {
        expectMatches(v, isIdenticalTo(w));
    });

    /**
     * This is a group of tests. You can look at it as a mini-suite of
     * tests, integrated in the overall suite defined by the file. A group
     * can be used to further reduce boiler-plate from test names
     * (the description of the group is pre-pended to the test description)
     * and (as you will see soon)
     */
    group("Inserting elements", [&]() {
        int* a;

        /**
         * This is an in-group setUp. It does NOT overwrite the global
         * setUp, but it is rather an addition to it. This will be executed
         * before each test in this group and its subgroups, after the
         * global setUp.
         *
         * In abstract, if you have the configuration:
         *
         * ```cpp
         * setUp(SetUpFunc0);
         * tearDown(TearDownFunc0);
         *
         * group(... {
         *   setUp(SetUpFunc1);
         *   tearDown(TearDownFunc1);
         *   group(...{
         *     ...
         *              test(TestFunc);
         *     ...
         *   }
         * });
         * ```
         *
         * then the order of functions called will be
         *
         * ```cpp
         * SetUpFunc0();
         * SetUpFunc1();
         * ...
         * SetUpFuncN();
         * TestFunc(); // execute the actual test
         * TearDownFuncN();
         * ...
         * TearDownFunc1();
         * TearDownFunc0();
         * ```
         */
        setUp([&]() {
            a = new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        });

        /**
         * This is the tearDown function. It is called after each test in the
         * group no matter whether it passed or failed. Generally, you can
         * use this space to clean up declared memory / delete created files
         * etc. Its behaviour is symmetrical to the setUp function.
         */
        tearDown([&]() {
            delete[] a;
        });

        test("Inserting a C-array of 10 elements - 0 (P)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expectMatches(v, isNotEmpty);

            expect(v[0] == 1);
            expectMatches(v[0], isEqualTo(1));

            expect(v.size() == 10);
            expectMatches(v, hasSize(10));
        });

        test("Inserting a C-array of 10 elements - 1 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expectMatches(v.empty(), isFalse);
            expectMatches(v, isNotEmpty);

            expect(v[0] == 2); // Fails!
            expectMatches(v[0], isEqualTo(1));

            expect(v.size() == 10);
            expectMatches(v, hasSize(10));
        });

        test("Inserting a C-array of 10 elements - 2 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expectMatches(v.empty(), isFalse);
            expectMatches(v, isNotEmpty);

            expect(v[0] == 1);
            expectMatches(v[0], isEqualTo(2)); // Fails!

            expect(v.size() == 10);
            expectMatches(v, hasSize(10));
        });

        test("Inserting a C-array of 10 elements - 3 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expectMatches(v, isNotEmpty);

            expect(v[0] == 1);
            expectMatches(v[0], isEqualTo(1));

            expect(v.size() == 10);
            expectMatches(v, hasSize(8)); // Fails!
        });

        test("Inserting one element in the middle - 0 (P)", [&]() {
            v = {10, 11, 12, 13, 14, 15, 16};
            v.insert(v.begin() + 5, 0);

            /**
             * Here are some examples of comparison matchers.
             *
             * These would fail at compile time if provided with un-matching
             * types (i.e. `expectMatches(2, isEqualTo({1, 2}))`);
             */
            expectMatches(v[5], isEqualTo(0));
            expectMatches(v[5], isLessThan(2));
            expectMatches(v[5], isLessThanOrEqualTo(2));
            expectMatches(v[5], isLessThanOrEqualTo(0));
            expectMatches(v[5], isGreaterThan(-2));
            expectMatches(v[5], isGreaterThanOrEqualTo(-2));
            expectMatches(v[5], isGreaterThanOrEqualTo(0));

            /**
             * Check THIS out! Matchers can be nested too!
             */
            expectMatches(v, hasSize(isGreaterThan(7)));

            /**
             * You can also have matchers for each element!
             */
            expectMatches(v, eachElement(isGreaterThanOrEqualTo(0)));
            expectMatches(v, anyElement(isEqualTo(13)));

            /**
             * You can compose matchers together through both, either and isNot.
             */
            expectMatches(v, eachElement(either(isZero, isGreaterThan(9))));
            expectMatches(v, anyElement(both(isOdd, isGreaterThan(10))));
            expectMatches(v, anyElement(both(isEven, isGreaterThan(10))));
            expectMatches(v, eachElement(isNot(isNegative)));
        });

        /**
         * The following group of tests will each fail one of the matchers
         * above, so you can see the auto-generated description of nested
         * matchers.
         */
        group("Failing versions of above test", [&]() {
            setUp([&]() { v = {10, 11, 12, 13, 14, 0, 15, 16}; });

            test("1 (F)", [&]() {
                expectMatches(v[5], isEqualTo(17));
            });

            test("2 (F)", [&]() {
                expectMatches(v[5], isLessThan(-2));
            });

            test("3 (F)", [&]() {
                expectMatches(v[5], isLessThanOrEqualTo(-2));
            });

            test("4 (F)", [&]() {
                expectMatches(v[5], isGreaterThan(2));
            });

            test("5 (F)", [&]() {
                expectMatches(v[5], isGreaterThanOrEqualTo(2));
            });

            test("6 (F)", [&]() {
                expectMatches(v, hasSize(isGreaterThan(7)));
            });

            test("7 (F)", [&]() {
                expectMatches(v, eachElement(isGreaterThanOrEqualTo(2)));
            });

            test("8 (F)", [&]() {
                expectMatches(v, anyElement(isEqualTo(22)));
            });

            test("9 (F)", [&]() {
                expectMatches(v, eachElement(either(isZero, isLessThan(9))));
            });

            test("10 (F)", [&]() {
                expectMatches(v, anyElement(both(isOdd, isGreaterThan(20))));
            });

            test("11 (F)", [&]() {
                expectMatches(v, eachElement(isNot(isPositive)));
            });
        });

        test("Inserting 5 equal elements (P)", [&]() {
            v.insert(v.end(), 5, 2);

            expectMatches(v, hasSize(5));
            expectMatches(v, eachElement(2));
        });

        test("Inserting 5 equal elements (F)", [&]() {
            v.insert(v.end(), 5, 2);

            expectMatches(v, hasSize(5));
            expectMatches(v, eachElement(3)); // Fails!
        });

        test("Inserting initializer list (P)", [&]() {
            v.insert(v.end(), {1, 2, 3, 4});

            expectMatches(v, hasSize(4));
            expectMatches(v, anyElement(3));
        });

        test("Inserting initializer list (F)", [&]() {
            v.insert(v.end(), {1, 2, 3, 4});

            expectMatches(v, hasSize(4));
            expectMatches(v, anyElement(7)); // Fails!
        });
    });

    group("Throwing", [&]() {
        test("Calling at for an out of bounds position throws - 0 (P)", [&]() {
            /**
             * You can also expect throws!
             */
            expectMatches([&]() { v.at(2); }, throws);
            expectMatches([&]() { v.at(2); }, throwsA<out_of_range>());
        });

        test("Calling at for an out of bounds position throws - 1 (F)", [&]() {
            v = {1, 2, 3};
            expectMatches([&]() { v.at(2); }, throws); // Fails!
        });

        test("Calling at for an out of bounds position throws - 2 (F)", [&]() {
            v = {1, 2, 3};
            expectMatches([&]() { v.at(2); },
                          throwsA<out_of_range>()); // Fails!
        });
    });

    /**
     * This is the only line of boiler-plate. This one is for integration with
     * automated tools: it automatically generates a JSON file describing all
     * tests executed, their structure and reasons for failure
     * and returns a status-code equal to the number of tests failed. Thus,
     * a passed suite of tests will return the exit code zero (0).
     */
    return finalizeTesting();
}
