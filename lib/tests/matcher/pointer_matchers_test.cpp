#include <kktest>
#include <kktest_matchers>

using namespace kktest;

class BasicClass {};
struct BasicStruct {};

int globalInt;
char globalChar;
BasicClass globalClass;
BasicStruct globalStruct;
double globalDouble;

void kkTestCase() {
    kkGroup("Null pointers", []() {
        int* intNullPtr = nullptr;
        char* charNullPtr = nullptr;
        BasicClass* classNullPtr = nullptr;
        BasicStruct* structNullPtr = nullptr;
        double* doubleNullPtr = nullptr;

        kkTest("isNull matcher matches nullptr", [&]() {
            kkExpect(isNull.matches(nullptr));
        });

        kkTest("isNull matcher matches null pointer of different types", [&]() {
            kkExpect(isNull.matches(intNullPtr));
            kkExpect(isNull.matches(charNullPtr));
            kkExpect(isNull.matches(classNullPtr));
            kkExpect(isNull.matches(structNullPtr));
            kkExpect(isNull.matches(doubleNullPtr));
        });

        kkTest("isNotNull matcher does not match nullptr", [&]() {
            kkExpect(!isNotNull.matches(nullptr));
        });

        kkTest("isNotNull matcher does not match null pointer of different types", [&]() {
            kkExpect(!isNotNull.matches(intNullPtr));
            kkExpect(!isNotNull.matches(charNullPtr));
            kkExpect(!isNotNull.matches(classNullPtr));
            kkExpect(!isNotNull.matches(structNullPtr));
            kkExpect(!isNotNull.matches(doubleNullPtr));
        });
    });

    kkGroup("Non-null pointers", []() {
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

        kkSetUp([&]() {
            intHeapPtr = new int();
            charHeapPtr = new char();
            classHeapPtr = new BasicClass();
            structHeapPtr = new BasicStruct();
            doubleHeapPtr = new double();
        });

        kkTearDown([&]() {
            delete intHeapPtr; intHeapPtr = nullptr;
            delete charHeapPtr; charHeapPtr = nullptr;
            delete classHeapPtr; classHeapPtr = nullptr;
            delete structHeapPtr; structHeapPtr = nullptr;
            delete doubleHeapPtr; doubleHeapPtr = nullptr;
        });

        kkTest("isNull does not match non-null heap pointers", [&]() {
            kkExpect(!isNull.matches(intHeapPtr));
            kkExpect(!isNull.matches(charHeapPtr));
            kkExpect(!isNull.matches(classHeapPtr));
            kkExpect(!isNull.matches(structHeapPtr));
            kkExpect(!isNull.matches(doubleHeapPtr));
        });

        kkTest("isNotNull matches non-null heap pointers", [&]() {
            kkExpect(isNotNull.matches(intHeapPtr));
            kkExpect(isNotNull.matches(charHeapPtr));
            kkExpect(isNotNull.matches(classHeapPtr));
            kkExpect(isNotNull.matches(structHeapPtr));
            kkExpect(isNotNull.matches(doubleHeapPtr));
        });

        kkTest("isNull does not match non-null stack pointers", [&]() {
            kkExpect(!isNull.matches(&stackInt));
            kkExpect(!isNull.matches(&stackChar));
            kkExpect(!isNull.matches(&stackClass));
            kkExpect(!isNull.matches(&stackStruct));
            kkExpect(!isNull.matches(&stackDouble));
        });

        kkTest("isNotNull matches non-null stack pointers", [&]() {
            kkExpect(isNotNull.matches(&stackInt));
            kkExpect(isNotNull.matches(&stackChar));
            kkExpect(isNotNull.matches(&stackClass));
            kkExpect(isNotNull.matches(&stackStruct));
            kkExpect(isNotNull.matches(&stackDouble));
        });

        kkTest("isNull does not match non-null stack pointers", [&]() {
            kkExpect(!isNull.matches(&globalInt));
            kkExpect(!isNull.matches(&globalChar));
            kkExpect(!isNull.matches(&globalClass));
            kkExpect(!isNull.matches(&globalStruct));
            kkExpect(!isNull.matches(&globalDouble));
        });

        kkTest("isNotNull matches non-null data segment pointers", [&]() {
            kkExpect(isNotNull.matches(&globalInt));
            kkExpect(isNotNull.matches(&globalChar));
            kkExpect(isNotNull.matches(&globalClass));
            kkExpect(isNotNull.matches(&globalStruct));
            kkExpect(isNotNull.matches(&globalDouble));
        });
    });
}
