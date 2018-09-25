#include "expect.hpp"

using namespace std;
using namespace matcher;

namespace runtime_testing {

ExpectationFailed::ExpectationFailed(const string& str):
        runtime_error(str) {}

ExpectationFailed::ExpectationFailed(const ExpectationFailed& other) noexcept:
        ExpectationFailed(other.getMessage()) {}

string ExpectationFailed::getMessage() const {
    return this->what();
}

void checkDuringTest(const char* fileName, const int& lineNumber) {
    if (!isDuringTest()) {
        throw runtime_error(
                string("'expect' can only be called inside tests! ") +
                "(" + fileName + ":" + to_string(lineNumber) + ")"
        );
    }
}

void throwExpectationFailed(
        Description* description,
        const char* fileName,
        const int& lineNumber) {
    description->append(
            string(" (") + fileName + ":" + to_string(lineNumber) + ")"
    );
    string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}

}
