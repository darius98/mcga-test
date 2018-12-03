#include <kktest.hpp>
#include <kktest_ext/core_matchers_impl/functional_matchers.hpp>

using kktest::expect;
using kktest::group;
using kktest::setUp;
using kktest::tearDown;
using kktest::test;
using kktest::core_matchers::throws;
using kktest::core_matchers::throwsA;
using kktest::core_matchers::wrapFunc;
using std::range_error;
using std::runtime_error;

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
    group("Lambdas", [] {
        test("throws matcher matches lambda throwing exception", [] {
            expect(throws.matches([] { throw runtime_error("Some error."); }));
        });

        test("throws matcher matches lambda throwing 5", [] {
            expect(throws.matches([] { throw 5; }));
        });

        test("throws matcher does not match lambda that does not throw", [] {
            expect(!throws.matches([] {}));
        });

        test("throwsA matcher matches lambda throwing exact error type", [] {
            expect(throwsA<runtime_error>().matches([] {
                throw runtime_error("Some error.");
            }));
        });

        test("throwsA matcher matches lambda throwing subtype", [] {
            expect(throwsA<runtime_error>().matches([] {
                throw range_error("Some range error.");
            }));
        });

        test("throwsA matcher does not match lambda throwing supertype", [] {
            expect(!throwsA<range_error>().matches([] {
                throw runtime_error("Some error.");
            }));
        });

        test("throwsA matcher does not match lambda throwing different types", [] {
            expect(!throwsA<runtime_error>().matches([] { throw 5; }));
        });

        test("throwsA matcher does not match lambda that does not throw", [] {
            expect(!throwsA<runtime_error>().matches([] {}));
        });
    });

    group("Simple global functions", [] {
        test("throws matcher matches function throwing an exception", [] {
            expect(throws.matches(simpleFunctionThrow));
        });

        test("throws matcher matches function throwing 5", [] {
            expect(throws.matches(simpleFunctionThrow5));
        });

        test("throws matcher does not match function not throwing", [] {
            expect(!throws.matches(simpleFunction));
        });

        test("throwsA matcher matches function throwing specific type", [] {
            expect(throwsA<runtime_error>().matches(simpleFunctionThrow));
            expect(throwsA<int>().matches(simpleFunctionThrow5));
        });

        test("throwsA matcher does not match function throwing different type", [] {
            expect(!throwsA<runtime_error>().matches(simpleFunctionThrow5));
            expect(!throwsA<int>().matches(simpleFunctionThrow));
        });

        test("throwsA matcher does not match function not throwing", [] {
            expect(!throwsA<runtime_error>().matches(simpleFunction));
            expect(!throwsA<int>().matches(simpleFunction));
        });
    });

    group("Wrapped global functions", [] {
        test("throws matcher matches wrapped function throwing an exception", [] {
            expect(throws.matches(wrapFunc(addThrow, 1, 1)));
        });

        test("throws matcher matches wrapped function throwing 5", [] {
            expect(throws.matches(wrapFunc(addThrow5, 1, 1)));
        });

        test("throws matcher does not match wrapped function not throwing", [] {
            expect(!throws.matches(wrapFunc(add, 1, 1)));
        });

        test("throwsA matcher matches wrapped function throwing specific type", [] {
            expect(throwsA<runtime_error>().matches(wrapFunc(addThrow, 1, 1)));
            expect(throwsA<int>().matches(wrapFunc(addThrow5, 1, 1)));
        });

        test("throwsA matcher does not match wrapped function throwing different type", [] {
            expect(!throwsA<runtime_error>().matches(wrapFunc(addThrow5, 1, 1)));
            expect(!throwsA<int>().matches(wrapFunc(addThrow, 1, 1)));
        });

        test("throwsA matcher does not match wrapped function not throwing", [] {
            expect(!throwsA<runtime_error>().matches(wrapFunc(add, 1, 1)));
            expect(!throwsA<int>().matches(wrapFunc(add, 1, 1)));
        });
    });

    group("Wrapped methods", [] {
        BasicClass* c = nullptr;

        setUp([&] { c = new BasicClass(1); });
        tearDown([&] { delete c; c = nullptr; });

        test("throws matcher matches wrapped method throwing an exception", [&] {
            expect(throws.matches(wrapFunc(&BasicClass::addWithThrow, c, 1)));
        });

        test("throws matcher matches wrapped method throwing 5", [&] {
            expect(throws.matches(wrapFunc(&BasicClass::addWithThrow5, c, 1)));
        });

        test("throws matcher does not match wrapped method not throwing", [&] {
            expect(!throws.matches(wrapFunc(&BasicClass::addWith, c, 1)));
        });

        test("throwsA matcher matches wrapped method throwing specific type", [&] {
            expect(throwsA<runtime_error>().matches(wrapFunc(&BasicClass::addWithThrow, c, 1)));
            expect(throwsA<int>().matches(wrapFunc(&BasicClass::addWithThrow5, c, 1)));
        });

        test("throwsA matcher does not match wrapped method throwing different type", [&] {
            expect(!throwsA<runtime_error>().matches(wrapFunc(&BasicClass::addWithThrow5, c, 1)));
            expect(!throwsA<int>().matches(wrapFunc(&BasicClass::addWithThrow, c, 1)));
        });

        test("throwsA matcher does not match wrapped method not throwing", [&] {
            expect(!throwsA<runtime_error>().matches(wrapFunc(&BasicClass::addWith, c, 1)));
            expect(!throwsA<int>().matches(wrapFunc(&BasicClass::addWith, c, 1)));
        });
    });
}
