#include "expect.hpp"

using namespace std;
using namespace matcher;

namespace runtime_testing {

ExpectationFailed::ExpectationFailed(const string& str):
        runtime_error(str) {}

ExpectationFailed::ExpectationFailed(const ExpectationFailed& other) noexcept:
        ExpectationFailed(other.getMessage()) {}

string ExpectationFailed::getMessage() const {
    return what();
}

void checkDuringTest(const char* fileName, const int& lineNumber) {
    if (!isDuringTest()) {
        throw runtime_error(
            string(fileName) + ":" + to_string(lineNumber) + ": "
            "'expect' can only be called inside tests!"
        );
    }
}

void throwExpectationFailed(Description* description) {
    string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}


void __expect(const bool& expr,
             const char* fileName,
             const int& lineNumber,
             const string& exprStr) {
    checkDuringTest(fileName, lineNumber);
    if (expr) {
        return;
    }
    auto description = matcher::Description::createForExpectation(
            fileName,
            lineNumber,
            exprStr
    );
    throwExpectationFailed(description);
}

void __fail(const string& message,
            const char* fileName,
            const int& lineNumber) {
    __expect(false, fileName, lineNumber, message);
}

}
