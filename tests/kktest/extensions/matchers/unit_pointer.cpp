#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/pointer.hpp"
#include "kktest/extensions/matchers/include/kktest_ext/matchers/testing_helpers.hpp"

using namespace mcga::test;
using namespace mcga::test::matchers;

class BasicClass {};
struct BasicStruct {};

int globalInt;
char globalChar;
BasicClass globalClass;
BasicStruct globalStruct;
double globalDouble;

TEST_CASE(pointer, "Matchers extension: pointer") {
    group("Null pointers", [] {
        int* intNullPtr = nullptr;
        BasicClass* classNullPtr = nullptr;
        BasicStruct* structNullPtr = nullptr;
        double* doubleNullPtr = nullptr;

        test("isNull matcher matches nullptr",
             [] { EXPECT_MATCHER_MATCHES(nullptr, isNull); });

        test("isNull matcher matches null pointer of different types", [&] {
            EXPECT_MATCHER_MATCHES(intNullPtr, isNull);
            EXPECT_MATCHER_MATCHES(classNullPtr, isNull);
            EXPECT_MATCHER_MATCHES(structNullPtr, isNull);
            EXPECT_MATCHER_MATCHES(doubleNullPtr, isNull);
        });

        test("isNotNull matcher does not match nullptr",
             [&] { EXPECT_MATCHER_FAILS(nullptr, isNotNull); });

        test("isNotNull matcher does not match null pointer of different types",
             [&] {
                 EXPECT_MATCHER_FAILS(intNullPtr, isNotNull);
                 EXPECT_MATCHER_FAILS(classNullPtr, isNotNull);
                 EXPECT_MATCHER_FAILS(structNullPtr, isNotNull);
                 EXPECT_MATCHER_FAILS(doubleNullPtr, isNotNull);
             });
    });

    group("Non-null pointers", [] {
        int* intHeapPtr = nullptr;
        char* charHeapPtr = nullptr;
        BasicClass* classHeapPtr = nullptr;
        BasicStruct* structHeapPtr = nullptr;
        double* doubleHeapPtr = nullptr;

        int stackInt;
        char stackChar;
        BasicClass stackClass;
        BasicStruct stackStruct;
        double stackDouble;

        setUp([&] {
            intHeapPtr = new int();
            charHeapPtr = new char();
            classHeapPtr = new BasicClass();
            structHeapPtr = new BasicStruct();
            doubleHeapPtr = new double();
        });

        tearDown([&] {
            delete intHeapPtr;
            intHeapPtr = nullptr;
            delete charHeapPtr;
            charHeapPtr = nullptr;
            delete classHeapPtr;
            classHeapPtr = nullptr;
            delete structHeapPtr;
            structHeapPtr = nullptr;
            delete doubleHeapPtr;
            doubleHeapPtr = nullptr;
        });

        test("isNull does not match non-null heap pointers", [&] {
            EXPECT_MATCHER_FAILS(intHeapPtr, isNull);
            EXPECT_MATCHER_FAILS(charHeapPtr, isNull);
            EXPECT_MATCHER_FAILS(classHeapPtr, isNull);
            EXPECT_MATCHER_FAILS(structHeapPtr, isNull);
            EXPECT_MATCHER_FAILS(doubleHeapPtr, isNull);
        });

        test("isNotNull matches non-null heap pointers", [&] {
            EXPECT_MATCHER_MATCHES(intHeapPtr, isNotNull);
            EXPECT_MATCHER_MATCHES(charHeapPtr, isNotNull);
            EXPECT_MATCHER_MATCHES(classHeapPtr, isNotNull);
            EXPECT_MATCHER_MATCHES(structHeapPtr, isNotNull);
            EXPECT_MATCHER_MATCHES(doubleHeapPtr, isNotNull);
        });

        test("isNull does not match non-null stack pointers", [&] {
            EXPECT_MATCHER_FAILS(&stackInt, isNull);
            EXPECT_MATCHER_FAILS(&stackChar, isNull);
            EXPECT_MATCHER_FAILS(&stackClass, isNull);
            EXPECT_MATCHER_FAILS(&stackStruct, isNull);
            EXPECT_MATCHER_FAILS(&stackDouble, isNull);
        });

        test("isNotNull matches non-null stack pointers", [&] {
            EXPECT_MATCHER_MATCHES(&stackInt, isNotNull);
            EXPECT_MATCHER_MATCHES(&stackChar, isNotNull);
            EXPECT_MATCHER_MATCHES(&stackClass, isNotNull);
            EXPECT_MATCHER_MATCHES(&stackStruct, isNotNull);
            EXPECT_MATCHER_MATCHES(&stackDouble, isNotNull);
        });

        test("isNull does not match non-null stack pointers", [&] {
            EXPECT_MATCHER_FAILS(&globalInt, isNull);
            EXPECT_MATCHER_FAILS(&globalChar, isNull);
            EXPECT_MATCHER_FAILS(&globalClass, isNull);
            EXPECT_MATCHER_FAILS(&globalStruct, isNull);
            EXPECT_MATCHER_FAILS(&globalDouble, isNull);
        });

        test("isNotNull matches non-null data segment pointers", [&] {
            EXPECT_MATCHER_MATCHES(&globalInt, isNotNull);
            EXPECT_MATCHER_MATCHES(&globalChar, isNotNull);
            EXPECT_MATCHER_MATCHES(&globalClass, isNotNull);
            EXPECT_MATCHER_MATCHES(&globalStruct, isNotNull);
            EXPECT_MATCHER_MATCHES(&globalDouble, isNotNull);
        });
    });
}
