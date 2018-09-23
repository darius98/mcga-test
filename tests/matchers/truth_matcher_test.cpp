#include "../../src/matcher/truth_matcher.hpp"

#include <iostream>

using namespace std;
using namespace testing::matcher;

int main() {
    bool isTrueMatchesTrue = isTrue->matches(true);
    cout << "isTrue called on 'true': "
         << (isTrueMatchesTrue ? "passed" : "failed")
         << ".\n";
    bool isTrueMatchesFalse = isTrue->matches(false);
    cout << "isTrue called on 'false': "
         << (isTrueMatchesFalse ? "failed" : "passed")
         << ".\n";
    bool isFalseMatchesTrue = isFalse->matches(true);
    cout << "isFalse called on 'true': "
         << (isFalseMatchesTrue ? "failed" : "passed")
         << ".\n";
    bool isFalseMatchesFalse = isFalse->matches(false);
    cout << "isFalse called on 'false': "
         << (isFalseMatchesFalse ? "passed" : "failed")
         << ".\n";
    return (!isTrueMatchesTrue)
           + (!isFalseMatchesFalse)
           + isTrueMatchesFalse
           + isFalseMatchesTrue;
}
