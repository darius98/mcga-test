#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/truth.hpp"

using namespace kktest;
using namespace kktest::matchers;

void truthMatchersTest() {
    test("isTrue matcher matches true", [] {
        expect(isTrue.matches(true));
    });

    test("isTrue matcher does not match false", [] {
        expect(!isTrue.matches(false));
    });

    test("isFalse matcher does not match true", [] {
        expect(!isFalse.matches(true));
    });

    test("isFalse matcher matches false", [] {
        expect(isFalse.matches(false));
    });
}
