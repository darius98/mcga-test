/**
 * This file provides a conceptual usage of available matchers / tools in the library. It is meant
 * to fail some tests (so you can see the descriptions you get when tests fail): actually, each
 * passing test has at least one corresponding failing test.
 *
 * Tests marked with (P) are intended to PASS, while tests marked with (F) are intended to fail, to
 * show their error message.
 */

#include <vector>

#include <kktest>
#include <kktest_matchers>

using namespace kktest;
using namespace std;


void kkTestCase() {
    vector<int> v, w;

    kkSetUp([&]() {
        v = vector<int>();
        w = vector<int>();
    });

    kkTest("Vector is initially empty - 0 (P)", [&]() {
        kkExpect(v.empty());
        kkExpect(v.empty(), isTrue);
        kkExpect(v, isEmpty);
    });

    kkTest("Vector is initially empty - 1 (F)", [&]() {
        // When the vector is not empty, the isEmpty expectation will fail.
        v = {1, 2, 3};

        kkExpect(v.empty()); // Fails!
    });

    kkTest(kkTestConfig($.description = "Vector is initially empty - 2 (F)",
                        $.optional = true), [&]() {
        // When the vector is not empty, the isEmpty expectation will fail.
        v = {1, 2, 3};

        kkExpect(v, isEmpty); // Fails!
    });

    kkTest("Two empty vectors are equal (P)", [&]() {
         kkExpect(v, isEqualTo(w));
    });

    kkTest("Two empty vectors are equal (F)", [&]() {
        w = {1};
        kkExpect(v, isEqualTo(w));
    });

    kkTest("Two vectors are identical (P)", [&]() {
        kkExpect(v, isIdenticalTo(v));
    });

    kkTest("Two vectors are identical (F)", [&]() {
        kkExpect(v, isIdenticalTo(w));
    });

    kkGroup("Inserting elements", [&]() {
        int* a;

        kkSetUp([&]() {
            a = new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        });

        kkTearDown([&]() {
            delete[] a;
        });

        kkTest("Inserting a C-array of 10 elements - 0 (P)", [&]() {
            v.insert(v.begin(), a, a + 10);

            kkExpect(!v.empty());
            kkExpect(v, isNotEmpty);

            kkExpect(v[0] == 1);
            kkExpect(v[0], isEqualTo(1));

            kkExpect(v.size() == 10);
            kkExpect(v, hasSize(10));
        });

        kkTest("Inserting a C-array of 10 elements - 1 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            kkExpect(!v.empty());
            kkExpect(v.empty(), isFalse);
            kkExpect(v, isNotEmpty);

            kkExpect(v[0] == 2); // Fails!
            kkExpect(v[0], isEqualTo(1));

            kkExpect(v.size() == 10);
            kkExpect(v, hasSize(10));
        });

        kkTest("Inserting a C-array of 10 elements - 2 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            kkExpect(!v.empty());
            kkExpect(v.empty(), isFalse);
            kkExpect(v, isNotEmpty);

            kkExpect(v[0] == 1);
            kkExpect(v[0], isEqualTo(2)); // Fails!

            kkExpect(v.size() == 10);
            kkExpect(v, hasSize(10));
        });

        kkTest("Inserting a C-array of 10 elements - 3 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            kkExpect(!v.empty());
            kkExpect(v, isNotEmpty);

            kkExpect(v[0] == 1);
            kkExpect(v[0], isEqualTo(1));

            kkExpect(v.size() == 10);
            kkExpect(v, hasSize(8)); // Fails!
        });

        kkTest("Inserting one element in the middle - 0 (P)", [&]() {
            v = {10, 11, 12, 13, 14, 15, 16};
            v.insert(v.begin() + 5, 0);

            kkExpect(v[5], isEqualTo(0));
            kkExpect(v[5], isLessThan(2));
            kkExpect(v[5], isLessThanEqual(2));
            kkExpect(v[5], isLessThanEqual(0));
            kkExpect(v[5], isGreaterThan(-2));
            kkExpect(v[5], isGreaterThanEqual(-2));
            kkExpect(v[5], isGreaterThanEqual(0));

            kkExpect(v, hasSize(isGreaterThan(7)));
            kkExpect(v, eachElement(isGreaterThanEqual(0)));
            kkExpect(v, anyElement(isEqualTo(13)));

            kkExpect(v, eachElement(either(isZero, isGreaterThan(9))));
            kkExpect(v, anyElement(both(isOdd, isGreaterThan(10))));
            kkExpect(v, anyElement(both(isEven, isGreaterThan(10))));
            kkExpect(v, eachElement(isNot(isNegative)));
        });

        kkGroup("Failing versions of above kkTest", [&]() {
            kkSetUp([&]() { v = {10, 11, 12, 13, 14, 0, 15, 16}; });

            kkTest("1 (F)", [&]() {
                kkExpect(v[5], isEqualTo(17));
            });

            kkTest("2 (F)", [&]() {
                kkExpect(v[5], isLessThan(-2));
            });

            kkTest("3 (F)", [&]() {
                kkExpect(v[5], isLessThanEqual(-2));
            });

            kkTest("4 (F)", [&]() {
                kkExpect(v[5], isGreaterThan(2));
            });

            kkTest("5 (F)", [&]() {
                kkExpect(v[5], isGreaterThanEqual(2));
            });

            kkTest("6 (F)", [&]() {
                kkExpect(v, eachElement(isGreaterThanEqual(2)));
            });

            kkTest("7 (F)", [&]() {
                kkExpect(v, anyElement(isEqualTo(22)));
            });

            kkTest("8 (F)", [&]() {
                kkExpect(v, eachElement(either(isZero, isLessThan(9))));
            });

            kkTest("9 (F)", [&]() {
                kkExpect(v, anyElement(both(isOdd, isGreaterThan(20))));
            });

            kkTest("10 (F)", [&]() {
                kkExpect(v, eachElement(isNot(isPositive)));
            });
        });

        kkTest("Inserting 5 equal elements (P)", [&]() {
            v.insert(v.end(), 5, 2);

            kkExpect(v, hasSize(5));
            kkExpect(v, eachElement(2));
        });

        kkTest("Inserting 5 equal elements (F)", [&]() {
            v.insert(v.end(), 5, 2);

            kkExpect(v, hasSize(5));
            kkExpect(v, eachElement(3)); // Fails!
        });

        kkTest("Inserting initializer list (P)", [&]() {
            v.insert(v.end(), {1, 2, 3, 4});

            kkExpect(v, hasSize(4));
            kkExpect(v, anyElement(3));
        });

        kkTest("Inserting initializer list (F)", [&]() {
            v.insert(v.end(), {1, 2, 3, 4});

            kkExpect(v, hasSize(4));
            kkExpect(v, anyElement(7)); // Fails!
        });
    });

    kkGroup("Throwing", [&]() {
        kkTest("Calling at for an out of bounds position throws - 0 (P)", [&]() {
            kkExpect([&]() { v.at(2); }, throws);
            kkExpect([&]() { v.at(2); }, throwsA<out_of_range>());
        });

        kkTest("Calling at for an out of bounds position throws - 1 (F)", [&]() {
            v = {1, 2, 3};
            kkExpect([&]() { v.at(2); }, throws); // Fails!
        });

        kkTest("Calling at for an out of bounds position throws - 2 (F)", [&]() {
            v = {1, 2, 3};
            kkExpect([&]() { v.at(2); }, throwsA<out_of_range>()); // Fails!
        });
    });

    kkTest(kkTestConfig($.description = "This kkTest should timeout (F)",
                        $.timeTicksLimit = 0.001,
                        $.optional = true), [&]() {
        for (int i = 0; i < 1000000; ++ i) {
            v.push_back(i);
        }
    });
}
