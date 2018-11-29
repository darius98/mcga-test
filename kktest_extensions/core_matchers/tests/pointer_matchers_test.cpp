#include <kktest>
#include <kktest_ext/core_matchers>

using namespace kktest;
using namespace kktest::core_matchers;

class BasicClass {};
struct BasicStruct {};

int globalInt;
char globalChar;
BasicClass globalClass;
BasicStruct globalStruct;
double globalDouble;

void kkTestCase(PointerMatchers) {
    group("Null pointers", [] {
        int* intNullPtr = nullptr;
        char* charNullPtr = nullptr;
        BasicClass* classNullPtr = nullptr;
        BasicStruct* structNullPtr = nullptr;
        double* doubleNullPtr = nullptr;

        test("isNull matcher matches nullptr", [] {
            expect(isNull.matches(nullptr));
        });

        test("isNull matcher matches null pointer of different types", [&] {
            expect(isNull.matches(intNullPtr));
            expect(isNull.matches(charNullPtr));
            expect(isNull.matches(classNullPtr));
            expect(isNull.matches(structNullPtr));
            expect(isNull.matches(doubleNullPtr));
        });

        test("isNotNull matcher does not match nullptr", [&] {
            expect(!isNotNull.matches(nullptr));
        });

        test("isNotNull matcher does not match null pointer of different types", [&] {
            expect(!isNotNull.matches(intNullPtr));
            expect(!isNotNull.matches(charNullPtr));
            expect(!isNotNull.matches(classNullPtr));
            expect(!isNotNull.matches(structNullPtr));
            expect(!isNotNull.matches(doubleNullPtr));
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
            delete intHeapPtr; intHeapPtr = nullptr;
            delete charHeapPtr; charHeapPtr = nullptr;
            delete classHeapPtr; classHeapPtr = nullptr;
            delete structHeapPtr; structHeapPtr = nullptr;
            delete doubleHeapPtr; doubleHeapPtr = nullptr;
        });

        test("isNull does not match non-null heap pointers", [&] {
            expect(!isNull.matches(intHeapPtr));
            expect(!isNull.matches(charHeapPtr));
            expect(!isNull.matches(classHeapPtr));
            expect(!isNull.matches(structHeapPtr));
            expect(!isNull.matches(doubleHeapPtr));
        });

        test("isNotNull matches non-null heap pointers", [&] {
            expect(isNotNull.matches(intHeapPtr));
            expect(isNotNull.matches(charHeapPtr));
            expect(isNotNull.matches(classHeapPtr));
            expect(isNotNull.matches(structHeapPtr));
            expect(isNotNull.matches(doubleHeapPtr));
        });

        test("isNull does not match non-null stack pointers", [&] {
            expect(!isNull.matches(&stackInt));
            expect(!isNull.matches(&stackChar));
            expect(!isNull.matches(&stackClass));
            expect(!isNull.matches(&stackStruct));
            expect(!isNull.matches(&stackDouble));
        });

        test("isNotNull matches non-null stack pointers", [&] {
            expect(isNotNull.matches(&stackInt));
            expect(isNotNull.matches(&stackChar));
            expect(isNotNull.matches(&stackClass));
            expect(isNotNull.matches(&stackStruct));
            expect(isNotNull.matches(&stackDouble));
        });

        test("isNull does not match non-null stack pointers", [&] {
            expect(!isNull.matches(&globalInt));
            expect(!isNull.matches(&globalChar));
            expect(!isNull.matches(&globalClass));
            expect(!isNull.matches(&globalStruct));
            expect(!isNull.matches(&globalDouble));
        });

        test("isNotNull matches non-null data segment pointers", [&] {
            expect(isNotNull.matches(&globalInt));
            expect(isNotNull.matches(&globalChar));
            expect(isNotNull.matches(&globalClass));
            expect(isNotNull.matches(&globalStruct));
            expect(isNotNull.matches(&globalDouble));
        });
    });
}
