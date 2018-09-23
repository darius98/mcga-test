#include "../../src/matcher/core_matchers.hpp"

#include <iostream>

using namespace std;
using namespace matcher;

int numFailed = 0;

string status(bool ok) {
    if (ok) {
        return "passed.\n";
    }
    numFailed += 1;
    return "failed.\n";
}

/// Note: `a` must be "less than" `c`, which must be "less than" `d`.
template<class T>
void testOnType(T a, T c, T d) {
    T b = a;
    auto matcher = equal(a);
    cout << "equality between equal values: "
         << status(matcher->matches(b));
    cout << "equality between unequal values: "
         << status(!matcher->matches(c));
    matcher = identical(a);
    cout << "identity between variable and itself: "
         << status(matcher->matches(a));
    cout << "identity between variable and equal variable: "
         << status(!matcher->matches(b));
    cout << "identity between variable and unequal variable: "
         << status(!matcher->matches(c));
    matcher = lessThan(c);
    cout << "lessThan when strictly less: "
         << status(matcher->matches(a));
    cout << "lessThan when equal: "
         << status(!matcher->matches(c));
    cout << "lessThan when strictly greater: "
         << status(!matcher->matches(d));
    matcher = lessThanOrEqualTo(c);
    cout << "lessThanOrEqualTo when strictly less: "
         << status(matcher->matches(a));
    cout << "lessThanOrEqualTo when equal: "
         << status(matcher->matches(c));
    cout << "lessThanOrEqualTo when strictly greater: "
         << status(!matcher->matches(d));
    matcher = greaterThan(c);
    cout << "greaterThan when strictly less: "
         << status(!matcher->matches(a));
    cout << "greaterThan when equal: "
         << status(!matcher->matches(c));
    cout << "greaterThan when strictly greater: "
         << status(matcher->matches(d));
    matcher = greaterThanOrEqualTo(c);
    cout << "greaterThanOrEqualTo when strictly less: "
         << status(!matcher->matches(a));
    cout << "greaterThanOrEqualTo when equal: "
         << status(matcher->matches(c));
    cout << "greaterThanOrEqualTo when strictly greater: "
         << status(matcher->matches(d));
}

int main() {
    testOnType<int>(3, 4, 5);
    testOnType<long>(3l, 4l, 5l);
    testOnType<long long>(3ll, 4ll, 5ll);
    testOnType<string>("a", "b", "c");
    testOnType<float>(2.717281f, 3.141529f, 6.234f);
    testOnType<double>(2.71728182, 3.14152965, 6.234);
    testOnType<long double>(2.71728182, 3.14152965, 6.234);
    testOnType<char>('x', 'y', 'z');
    return numFailed;
}
