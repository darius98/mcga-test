#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/functional.hpp"
#include "kktest/extensions/matchers/include/kktest_ext/matchers/testing_helpers.hpp"

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
            EXPECT_MATCHER_MATCHES([] {
                throw runtime_error("Some error.");
            }, throws);
        });

        test("throws matcher matches lambda throwing 5", [] {
            expect([] { throw 5; }, throws);
        });

        test("throws matcher does not match lambda that does not throw", [] {
            EXPECT_MATCHER_FAILS([] {}, throws);
        });

        test("throwsA matcher matches lambda throwing exact error type", [] {
            EXPECT_MATCHER_MATCHES([] {
                throw runtime_error("Some error.");
            }, throwsA<runtime_error>());
        });

        test("throwsA matcher matches lambda throwing subtype", [] {
            EXPECT_MATCHER_FAILS([] {
                throw range_error("Some range error.");
            }, throwsA<runtime_error>());
        });

        test("throwsA matcher does not match lambda throwing supertype", [] {
            EXPECT_MATCHER_FAILS([] {
                throw runtime_error("Some error.");
            }, throwsA<range_error>());
        });

        test("throwsA matcher does not match lambda throwing different types",
             [] {
            EXPECT_MATCHER_FAILS([] { throw 5; }, throwsA<runtime_error>());
        });

        test("throwsA matcher does not match lambda that does not throw", [] {
            EXPECT_MATCHER_FAILS([] {}, throwsA<runtime_error>());
        });
    });

    group("Simple global functions", [] {
        test("throws matcher matches function throwing an exception", [] {
            EXPECT_MATCHER_MATCHES(simpleFunctionThrow, throws);
        });

        test("throws matcher matches function throwing 5", [] {
            EXPECT_MATCHER_MATCHES(simpleFunctionThrow5, throws);
        });

        test("throws matcher does not match function not throwing", [] {
            EXPECT_MATCHER_FAILS(simpleFunction, throws);
        });

        test("throwsA matcher matches function throwing specific type", [] {
            EXPECT_MATCHER_MATCHES(simpleFunctionThrow,
                                   throwsA<runtime_error>());
            EXPECT_MATCHER_MATCHES(simpleFunctionThrow5, throwsA<int>());
        });

        test("throwsA matcher does not match function throwing different type",
             [] {
            EXPECT_MATCHER_FAILS(simpleFunctionThrow5,
                                 throwsA<runtime_error>());
            EXPECT_MATCHER_FAILS(simpleFunctionThrow, throwsA<int>());
        });

        test("throwsA matcher does not match function not throwing", [] {
            EXPECT_MATCHER_FAILS(simpleFunction, throwsA<runtime_error>());
            EXPECT_MATCHER_FAILS(simpleFunction, throwsA<int>());
        });
    });
}
