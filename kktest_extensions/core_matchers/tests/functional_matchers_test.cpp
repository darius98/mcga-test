#include <kktest>
#include <kktest_ext/core_matchers>

using namespace kktest::core_matchers;
using namespace std;

class BasicClass {
public:
    int x;

    explicit BasicClass(int _x): x(_x) {}

    int addWith(int y) { return x + y; }

    int addWithThrow(int y) {
        throw runtime_error("Some error.");
    }

    int addWithThrow5(int y) {
        throw 5;
    }
};

int add(int x, int y) {
    return x + y;
}

int addThrow(int, int) {
    throw runtime_error("Some error.");
}

int addThrow5(int, int) {
    throw 5;
}

void simpleFunction() {}

void simpleFunctionThrow() {
    throw runtime_error("Some error.");
}

void simpleFunctionThrow5() {
    throw 5;
}


void kkTestCase(FunctionalMatchers) {
    kkGroup("Lambdas", []() {
        kkTest("throws matcher matches lambda throwing exception", []() {
            kkExpect(throws.matches([]() { throw runtime_error("Some error."); }));
        });

        kkTest("throws matcher matches lambda throwing 5", []() {
            kkExpect(throws.matches([]() { throw 5; }));
        });

        kkTest("throws matcher does not match lambda that does not throw", []() {
            kkExpect(!throws.matches([]() {}));
        });

        kkTest("throwsA matcher matches lambda throwing exact error type", []() {
            kkExpect(throwsA<runtime_error>().matches([]() {
                throw runtime_error("Some error.");
            }));
        });

        kkTest("throwsA matcher matches lambda throwing subtype", []() {
            kkExpect(throwsA<runtime_error>().matches([]() {
                throw range_error("Some range error.");
            }));
        });

        kkTest("throwsA matcher does not match lambda throwing supertype", []() {
            kkExpect(!throwsA<range_error>().matches([]() {
                throw runtime_error("Some error.");
            }));
        });

        kkTest("throwsA matcher does not match lambda throwing different types", []() {
            kkExpect(!throwsA<runtime_error>().matches([]() { throw 5; }));
        });

        kkTest("throwsA matcher does not match lambda that does not throw", []() {
            kkExpect(!throwsA<runtime_error>().matches([]() {}));
        });
    });

    kkGroup("Simple global functions", []() {
        kkTest("throws matcher matches function throwing an exception", []() {
            kkExpect(throws.matches(simpleFunctionThrow));
        });

        kkTest("throws matcher matches function throwing 5", []() {
            kkExpect(throws.matches(simpleFunctionThrow5));
        });

        kkTest("throws matcher does not match function not throwing", []() {
            kkExpect(!throws.matches(simpleFunction));
        });

        kkTest("throwsA matcher matches function throwing specific type", []() {
            kkExpect(throwsA<runtime_error>().matches(simpleFunctionThrow));
            kkExpect(throwsA<int>().matches(simpleFunctionThrow5));
        });

        kkTest("throwsA matcher does not match function throwing different type", []() {
            kkExpect(!throwsA<runtime_error>().matches(simpleFunctionThrow5));
            kkExpect(!throwsA<int>().matches(simpleFunctionThrow));
        });

        kkTest("throwsA matcher does not match function not throwing", []() {
            kkExpect(!throwsA<runtime_error>().matches(simpleFunction));
            kkExpect(!throwsA<int>().matches(simpleFunction));
        });
    });

    kkGroup("Wrapped global functions", []() {
        kkTest("throws matcher matches wrapped function throwing an exception", []() {
            kkExpect(throws.matches(wrapFunc(addThrow, 1, 1)));
        });

        kkTest("throws matcher matches wrapped function throwing 5", []() {
            kkExpect(throws.matches(wrapFunc(addThrow5, 1, 1)));
        });

        kkTest("throws matcher does not match wrapped function not throwing", []() {
            kkExpect(!throws.matches(wrapFunc(add, 1, 1)));
        });

        kkTest("throwsA matcher matches wrapped function throwing specific type", []() {
            kkExpect(throwsA<runtime_error>().matches(wrapFunc(addThrow, 1, 1)));
            kkExpect(throwsA<int>().matches(wrapFunc(addThrow5, 1, 1)));
        });

        kkTest("throwsA matcher does not match wrapped function throwing different type", []() {
            kkExpect(!throwsA<runtime_error>().matches(wrapFunc(addThrow5, 1, 1)));
            kkExpect(!throwsA<int>().matches(wrapFunc(addThrow, 1, 1)));
        });

        kkTest("throwsA matcher does not match wrapped function not throwing", []() {
            kkExpect(!throwsA<runtime_error>().matches(wrapFunc(add, 1, 1)));
            kkExpect(!throwsA<int>().matches(wrapFunc(add, 1, 1)));
        });
    });

    kkGroup("Wrapped methods", []() {
        BasicClass* c = nullptr;

        kkSetUp([&]() { c = new BasicClass(1); });
        kkTearDown([&]() { delete c; c = nullptr; });

        kkTest("throws matcher matches wrapped method throwing an exception", [&]() {
            kkExpect(throws.matches(wrapFunc(&BasicClass::addWithThrow, c, 1)));
        });

        kkTest("throws matcher matches wrapped method throwing 5", [&]() {
            kkExpect(throws.matches(wrapFunc(&BasicClass::addWithThrow5, c, 1)));
        });

        kkTest("throws matcher does not match wrapped method not throwing", [&]() {
            kkExpect(!throws.matches(wrapFunc(&BasicClass::addWith, c, 1)));
        });

        kkTest("throwsA matcher matches wrapped method throwing specific type", [&]() {
            kkExpect(throwsA<runtime_error>().matches(wrapFunc(&BasicClass::addWithThrow, c, 1)));
            kkExpect(throwsA<int>().matches(wrapFunc(&BasicClass::addWithThrow5, c, 1)));
        });

        kkTest("throwsA matcher does not match wrapped method throwing different type", [&]() {
            kkExpect(!throwsA<runtime_error>().matches(wrapFunc(&BasicClass::addWithThrow5, c, 1)));
            kkExpect(!throwsA<int>().matches(wrapFunc(&BasicClass::addWithThrow, c, 1)));
        });

        kkTest("throwsA matcher does not match wrapped method not throwing", [&]() {
            kkExpect(!throwsA<runtime_error>().matches(wrapFunc(&BasicClass::addWith, c, 1)));
            kkExpect(!throwsA<int>().matches(wrapFunc(&BasicClass::addWith, c, 1)));
        });
    });
}
