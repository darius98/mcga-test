#include "expect_matches.hpp"

using namespace std;


namespace kktest {

void ExpectMatchesDefiner::throwExpectationFailed(Description* description) {
    string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}

}
