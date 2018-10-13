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

#include <kktest_matchers>
#include <kktest>

using namespace kktest;
using namespace std;


void testCase() {
    vector<int> v, w;

    setUp([&]() {
        v = vector<int>();
        w = vector<int>();
    });

    test("Vector is initially empty - 0 (P)", [&]() {
        expect(v.empty());
        expect(v.empty(), isTrue);
        expect(v, isEmpty);
    });

    test("Vector is initially empty - 1 (F)", [&]() {
        // When the vector is not empty, the isEmpty expectation will fail.
        v = {1, 2, 3};

        expect(v.empty()); // Fails!
    });

    test("Vector is initially empty - 2 (F)", [&]() {
        // When the vector is not empty, the isEmpty expectation will fail.
        v = {1, 2, 3};

        expect(v, isEmpty); // Fails!
    });

    test("Two empty vectors are equal (P)", [&]() {
         expect(v, isEqualTo(w));
    });

    test("Two empty vectors are equal (F)", [&]() {
        w = {1};
        expect(v, isEqualTo(w));
    });

    test("Two vectors are identical (P)", [&]() {
        expect(v, isIdenticalTo(v));
    });

    test("Two vectors are identical (F)", [&]() {
        expect(v, isIdenticalTo(w));
    });

    group("Inserting elements", [&]() {
        int* a;

        setUp([&]() {
            a = new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        });

        tearDown([&]() {
            delete[] a;
        });

        test("Inserting a C-array of 10 elements - 0 (P)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expect(v, isNotEmpty);

            expect(v[0] == 1);
            expect(v[0], isEqualTo(1));

            expect(v.size() == 10);
            expect(v, hasSize(10));
        });

        test("Inserting a C-array of 10 elements - 1 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expect(v.empty(), isFalse);
            expect(v, isNotEmpty);

            expect(v[0] == 2); // Fails!
            expect(v[0], isEqualTo(1));

            expect(v.size() == 10);
            expect(v, hasSize(10));
        });

        test("Inserting a C-array of 10 elements - 2 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expect(v.empty(), isFalse);
            expect(v, isNotEmpty);

            expect(v[0] == 1);
            expect(v[0], isEqualTo(2)); // Fails!

            expect(v.size() == 10);
            expect(v, hasSize(10));
        });

        test("Inserting a C-array of 10 elements - 3 (F)", [&]() {
            v.insert(v.begin(), a, a + 10);

            expect(!v.empty());
            expect(v, isNotEmpty);

            expect(v[0] == 1);
            expect(v[0], isEqualTo(1));

            expect(v.size() == 10);
            expect(v, hasSize(8)); // Fails!
        });

        test("Inserting one element in the middle - 0 (P)", [&]() {
            v = {10, 11, 12, 13, 14, 15, 16};
            v.insert(v.begin() + 5, 0);

            expect(v[5], isEqualTo(0));
            expect(v[5], isLessThan(2));
            expect(v[5], isLessThanEqual(2));
            expect(v[5], isLessThanEqual(0));
            expect(v[5], isGreaterThan(-2));
            expect(v[5], isGreaterThanEqual(-2));
            expect(v[5], isGreaterThanEqual(0));

            expect(v, hasSize(isGreaterThan(7)));
            expect(v, eachElement(isGreaterThanEqual(0)));
            expect(v, anyElement(isEqualTo(13)));

            expect(v, eachElement(either(isZero, isGreaterThan(9))));
            expect(v, anyElement(both(isOdd, isGreaterThan(10))));
            expect(v, anyElement(both(isEven, isGreaterThan(10))));
            expect(v, eachElement(isNot(isNegative)));
        });

        group("Failing versions of above test", [&]() {
            setUp([&]() { v = {10, 11, 12, 13, 14, 0, 15, 16}; });

            test("1 (F)", [&]() {
                expect(v[5], isEqualTo(17));
            });

            test("2 (F)", [&]() {
                expect(v[5], isLessThan(-2));
            });

            test("3 (F)", [&]() {
                expect(v[5], isLessThanEqual(-2));
            });

            test("4 (F)", [&]() {
                expect(v[5], isGreaterThan(2));
            });

            test("5 (F)", [&]() {
                expect(v[5], isGreaterThanEqual(2));
            });

            test("6 (F)", [&]() {
                expect(v, eachElement(isGreaterThanEqual(2)));
            });

            test("7 (F)", [&]() {
                expect(v, anyElement(isEqualTo(22)));
            });

            test("8 (F)", [&]() {
                expect(v, eachElement(either(isZero, isLessThan(9))));
            });

            test("9 (F)", [&]() {
                expect(v, anyElement(both(isOdd, isGreaterThan(20))));
            });

            test("10 (F)", [&]() {
                expect(v, eachElement(isNot(isPositive)));
            });
        });

        test("Inserting 5 equal elements (P)", [&]() {
            v.insert(v.end(), 5, 2);

            expect(v, hasSize(5));
            expect(v, eachElement(2));
        });

        test("Inserting 5 equal elements (F)", [&]() {
            v.insert(v.end(), 5, 2);

            expect(v, hasSize(5));
            expect(v, eachElement(3)); // Fails!
        });

        test("Inserting initializer list (P)", [&]() {
            v.insert(v.end(), {1, 2, 3, 4});

            expect(v, hasSize(4));
            expect(v, anyElement(3));
        });

        test("Inserting initializer list (F)", [&]() {
            v.insert(v.end(), {1, 2, 3, 4});

            expect(v, hasSize(4));
            expect(v, anyElement(7)); // Fails!
        });
    });

    group("Throwing", [&]() {
        test("Calling at for an out of bounds position throws - 0 (P)", [&]() {
            expect([&]() { v.at(2); }, throws);
            expect([&]() { v.at(2); }, throwsA<out_of_range>());
        });

        test("Calling at for an out of bounds position throws - 1 (F)", [&]() {
            v = {1, 2, 3};
            expect([&]() { v.at(2); }, throws); // Fails!
        });

        test("Calling at for an out of bounds position throws - 2 (F)", [&]() {
            v = {1, 2, 3};
            expect([&]() { v.at(2); }, throwsA<out_of_range>()); // Fails!
        });
    });
}
