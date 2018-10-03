#include "expect.hpp"

using namespace std;
using namespace ktest;

namespace ktest {

ExpectationFailed::ExpectationFailed(const string& str):
        runtime_error(str) {}

ExpectationFailed::ExpectationFailed(const ExpectationFailed& other) noexcept:
        ExpectationFailed(other.getMessage()) {}

string ExpectationFailed::getMessage() const {
    return what();
}

void checkDuringTest(const string& file, int line) {
    if (!isDuringTest()) {
        throw runtime_error(
            file + ":" + to_string(line) + ": "
            "'expect' can only be called inside tests!"
        );
    }
}

void throwExpectationFailed(Description* description) {
    string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}


void __expect(const bool& exprResult,
              const string& file,
              int line,
              const string& expr) {
    checkDuringTest(file, line);
    if (exprResult) {
        return;
    }
    throwExpectationFailed(
        Description::createForExpect(file, line, expr)
    );
}

}
