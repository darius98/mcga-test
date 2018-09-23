#include "../../src/matcher/truth_matcher.hpp"

#include <iostream>

using namespace std;
using namespace testing::matcher;

int main() {
    cout << "Testing isTrue matcher:\n" << boolalpha;
    bool isTrueMatchesTrue = isTrue->matches(true);
    cout << "\tisTrue called on 'true': " << isTrueMatchesTrue << "\n";
    bool isTrueMatchesFalse = isTrue->matches(false);
    cout << "\tisTrue called on 'false': " << isTrueMatchesFalse << "\n";
    bool isFalseMatchesTrue = isFalse->matches(true);
    cout << "\tisFalse called on 'true': " << isFalseMatchesTrue << "\n";
    bool isFalseMatchesFalse = isFalse->matches(false);
    cout << "\tisFalse called on 'false': " << isFalseMatchesFalse << "\n";
    return (!isTrueMatchesTrue)
           + (!isFalseMatchesFalse)
           + isTrueMatchesFalse
           + isFalseMatchesTrue;
}
