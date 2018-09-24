#include "../../src/matcher/core_matchers.hpp"

#include "matcher_test.hpp"

using namespace std;
using namespace matcher;

template<class T>
string S(const T& t) {
    stringstream s;
    s << t;
    return s.str();
}

/// Note: `a` must be "less than" `c`, which must be "less than" `d`.
template<class T>
void testOnType(T a, T c, T d) {
    T b = a;
    auto matcher = isEqualTo(a);
    test("equality between equal values", b, matcher);
    test("equality between unequal values", c, matcher, false,
         "'" + S(a) + "', got '" + S(c) + "'"
    );
    matcher = isIdenticalTo(a);
    test("identity between variable and itself", a, matcher);
    test("identity between variable and equal variable", b, matcher, false);
    test("identity between variable and unequal variable", c, matcher, false);
    matcher = isLessThan(c);
    test("isLessThan when strictly less", a, matcher);
    test("isLessThan when equal", c, matcher, false,
         "less than '" + S(c) + "', got '" + S(c) + "'"
    );
    test("isLessThan when strictly greater", d, matcher, false,
         "less than '" + S(c) + "', got '" + S(d) + "'"
    );
    matcher = isLessThanOrEqualTo(c);
    test("isLessThanOrEqualTo when strictly less", a, matcher);
    test("isLessThanOrEqualTo when equal", c, matcher);
    test("isLessThanOrEqualTo when strictly greater", d, matcher, false,
         "less than or equal to '" + S(c) + "', got '" + S(d) + "'"
    );
    matcher = isGreaterThan(c);
    test("isGreaterThan when strictly less", a, matcher, false,
         "greater than '" + S(c) + "', got '" + S(a) + "'"
    );
    test("isGreaterThan when equal", c, matcher, false,
         "greater than '" + S(c) + "', got '" + S(c) + "'"
    );
    test("isGreaterThan when strictly greater", d, matcher);
    matcher = isGreaterThanOrEqualTo(c);
    test("isGreaterThanOrEqualTo when strictly less", a, matcher, false,
         "greater than or equal to '" + S(c) + "', got '" + S(a) + "'"
    );
    test("isGreaterThanOrEqualTo when equal", c, matcher);
    test("isGreaterThanOrEqualTo when strictly greater", d, matcher);
}

void comparisonMatcherTest() {
    testOnType<int>(3, 4, 5);
    testOnType<long>(3l, 4l, 5l);
    testOnType<long long>(3ll, 4ll, 5ll);
    testOnType<string>("a", "b", "c");
    testOnType<float>(2.71f, 3.14f, 6.2f);
    testOnType<double>(2.71, 3.14, 6.2);
    testOnType<long double>(2.71, 3.14, 6.2);
    testOnType<char>('x', 'y', 'z');
}
