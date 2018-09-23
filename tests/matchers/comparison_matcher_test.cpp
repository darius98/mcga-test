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

int numFailed = 0;

/// Note: `a` must be "less than" `c`, which must be "less than" `d`.
template<class T>
void testOnType(T a, T c, T d) {
    T b = a;
    auto matcher = equal(a);
    cout << "equality between equal values: ";
    numFailed += !expectMatch(b, matcher);
    cout << "equality between unequal values: ";
    numFailed += !expectMatch(c, matcher, false,
            "'" + S(a) + "', got '" + S(c) + "'"
    );
    matcher = identical(a);
    cout << "identity between variable and itself: ";
    numFailed += !expectMatch(a, matcher);
    cout << "identity between variable and equal variable: ";
    numFailed += !expectMatch(b, matcher, false);
    cout << "identity between variable and unequal variable: ";
    numFailed += !expectMatch(c, matcher, false);
    matcher = lessThan(c);
    cout << "lessThan when strictly less: ";
    numFailed += !expectMatch(a, matcher);
    cout << "lessThan when equal: ";
    numFailed += !expectMatch(c, matcher, false,
            "less than '" + S(c) + "', got '" + S(c) + "'"
    );
    cout << "lessThan when strictly greater: ";
    numFailed += !expectMatch(d, matcher, false,
            "less than '" + S(c) + "', got '" + S(d) + "'"
    );
    matcher = lessThanOrEqualTo(c);
    cout << "lessThanOrEqualTo when strictly less: ";
    numFailed += !expectMatch(a, matcher);
    cout << "lessThanOrEqualTo when equal: ";
    numFailed += !expectMatch(c, matcher);
    cout << "lessThanOrEqualTo when strictly greater: ";
    numFailed += !expectMatch(d, matcher, false,
            "less than or equal to '" + S(c) + "', got '" + S(d) + "'"
    );
    matcher = greaterThan(c);
    cout << "greaterThan when strictly less: ";
    numFailed += !expectMatch(a, matcher, false,
            "greater than '" + S(c) + "', got '" + S(a) + "'"
    );
    cout << "greaterThan when equal: ";
    numFailed += !expectMatch(c, matcher, false,
            "greater than '" + S(c) + "', got '" + S(c) + "'"
    );
    cout << "greaterThan when strictly greater: ";
    numFailed += !expectMatch(d, matcher);
    matcher = greaterThanOrEqualTo(c);
    cout << "greaterThanOrEqualTo when strictly less: ";
    numFailed += !expectMatch(a, matcher, false,
            "greater than or equal to '" + S(c) + "', got '" + S(a) + "'"
    );
    cout << "greaterThanOrEqualTo when equal: ";
    numFailed += !expectMatch(c, matcher);
    cout << "greaterThanOrEqualTo when strictly greater: ";
    numFailed += !expectMatch(d, matcher);
}

int main() {
    testOnType<int>(3, 4, 5);
    testOnType<long>(3l, 4l, 5l);
    testOnType<long long>(3ll, 4ll, 5ll);
    testOnType<string>("a", "b", "c");
    testOnType<float>(2.71f, 3.14f, 6.2f);
    testOnType<double>(2.71, 3.14, 6.2);
    testOnType<long double>(2.71, 3.14, 6.2);
    testOnType<char>('x', 'y', 'z');
    return numFailed;
}
