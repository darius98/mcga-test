#include "../../src/matcher/equality_matcher.hpp"

#include <iostream>

using namespace std;
using namespace testing::matcher;

template<class T>
int testOnType(T a, T c) {
    T b = a;
    int failed = 0;
    Matcher<T>* matcher = new EqualityMatcher<T>(a, defaultEquality<T>);
    cout << "equality (using direct class) between '"
         << a
         << "' and '"
         << b
         << "': ";
    if (!matcher->matches(b)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    cout << "equality (using direct class) between '"
         << a
         << "' and '"
         << c
         << "': ";
    if (matcher->matches(c)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    matcher = equals(a);
    cout << "equality (using function equals) between a and a: ";
    if (!matcher->matches(b)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    cout << "equality (using function equals) between a and c: ";
    if (matcher->matches(c)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    matcher = new EqualityMatcher<T>(a, identityEquality<T>);
    cout << "identity (using direct class) between variable and itself: ";
    if (!matcher->matches(a)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    cout << "identity (using direct class) between variable and other variable "
            "with same value: ";
    if (matcher->matches(b)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    cout << "identity (using direct class) between variable and other variable "
            "with different value: ";
    if (matcher->matches(c)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    matcher = isIdentical(a);
    cout << "identity (using direct class) between variable and itself: ";
    if (!matcher->matches(a)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    cout << "identity (using direct class) between variable and other variable "
            "with same value: ";
    if (matcher->matches(b)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    cout << "identity (using direct class) between variable and other variable"
            "with different value: ";
    if (matcher->matches(c)) {
        failed += 1;
        cout << "failed.\n";
    } else {
        cout << "passed.\n";
    }
    return failed;
}

int main() {
    int failed = 0;
    failed += testOnType<int>(3, 4);
    failed += testOnType<long>(3l, 4l);
    failed += testOnType<long long>(3ll, 4ll);
    failed += testOnType<string>("a", "b");
    failed += testOnType<float>(2.717281f, 3.141529f);
    failed += testOnType<double>(2.71728182, 3.14152965);
    failed += testOnType<long double>(2.71728182, 3.14152965);
    failed += testOnType<char>('x', 'y');
    return failed;
}
