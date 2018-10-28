#include <core/driver.hpp>

#include "expect_matches.hpp"

using namespace std;


namespace kktest {

void ExpectMatchesDefiner::operator()(const bool& result, const string& expr) {
    ExpectDefiner::operator()(result, expr);
}

void ExpectMatchesDefiner::throwExpectationFailed(Description* description) {
    string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}

void ExpectMatchesDefiner::checkDuringTest() {
    if (!TestingDriver::isDuringTest()) {
        throw runtime_error(
            file + ":" + to_string(line) + ": "
            "'expect' can only be called inside tests!"
        );
    }
}

}
