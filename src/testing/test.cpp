#include "test.hpp"
#include "group.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

Test::Test(string _description, string _file, int _line):
        description(move(_description)), file(move(_file)), line(_line) {}

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

string Test::getFullDescription() const {
    string fullDescription = file + ":" + to_string(line) + ": ";

    if (parentGroup->parentGroup != nullptr) {
        fullDescription += parentGroup->getFullDescription() + "::";
    }

    return fullDescription + description;
}

JSON Test::generateReport() const {
    JSON report = {
        {"description", description},
        {"file", file},
        {"line", line},
        {"passed", isPassed()}
    };
    if (!isPassed()) {
        report["failureMessage"] = getFailureMessage();
    }
    return report;
}

}
