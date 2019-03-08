#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/functional.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace std;

void simpleFunction() {}

void simpleFunctionThrow() {
    throw runtime_error("Some error.");
}

void simpleFunctionThrow5() {
    throw 5;
}


void functionalMatchersTest() {
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

        test("throwsA matcher does not match lambda throwing different types",
             [] {
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

        test("throwsA matcher does not match function throwing different type",
             [] {
            expect(!throwsA<runtime_error>().matches(simpleFunctionThrow5));
            expect(!throwsA<int>().matches(simpleFunctionThrow));
        });

        test("throwsA matcher does not match function not throwing", [] {
            expect(!throwsA<runtime_error>().matches(simpleFunction));
            expect(!throwsA<int>().matches(simpleFunction));
        });
    });
}
