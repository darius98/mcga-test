#include "test.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

Test::Test(string description, string file, int line):
        description(move(description)), file(move(file)), line(line) {}

Test::~Test() {
    delete failure;
}

bool Test::isFailed() const {
    return failure != nullptr;
}

bool Test::isPassed() const {
    return !isFailed();
}

void Test::setFailure(const string& message) {
    if (isPassed()) {
        failure = new ExpectationFailed(message);
    }
}

void Test::setFailure(const ExpectationFailed& f) {
    if (isPassed()) {
        failure = new ExpectationFailed(f);
    }
}

string Test::getFailureMessage() const {
    return failure->getMessage();
}

JSON Test::generateReport() const {
    JSON report;
    report["description"] = description;
    if (!file.empty()) {
        report["file"] = file;
        report["line"] = line;
    }
    report["passed"] = isPassed();
    if (!isPassed()) {
        report["failureMessage"] = getFailureMessage();
    }
    return report;
}

}
