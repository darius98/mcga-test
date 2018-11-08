#include <cstring>

#include <utils/unescape_characters.hpp>
#include "test.hpp"
#include "group.hpp"

using namespace autojson;
using namespace std;
using namespace kktest::utils;


namespace kktest {

int Test::globalTestIndex = 0;

Test::Test(string _description, string _file, int _line, Group* _parentGroup):
        description(move(_description)),
        file(move(_file)),
        line(_line),
        parentGroup(_parentGroup),
        index(++globalTestIndex) {}

Test::~Test() {
    delete failure;
}

int Test::getIndex() const {
    return index;
}

string Test::getFilename() const {
    return file;
}

int Test::getLine() const {
    return line;
}

void Test::setExecuted() {
    if (isExecuted()) {
        throw runtime_error("Test::setExecuted called twice on the same test!");
    }
    executed = true;
}

bool Test::isExecuted() const {
    return executed;
}

bool Test::isFailed() const {
    return isExecuted() && failure != nullptr;
}

bool Test::isPassed() const {
    return isExecuted() && !isFailed();
}

void Test::setFailure(const string& message) {
    if (failure == nullptr) {
        failure = new ExpectationFailed(message);
    }
}

void Test::setFailure(const ExpectationFailed& f) {
    if (failure == nullptr) {
        failure = new ExpectationFailed(f);
    }
}

string Test::getFailureMessage() const {
    return failure->getMessage();
}

string Test::getDescriptionPrefix() const {
    string prefix = file + ":" + to_string(line) + "::";
    if (!parentGroup->isGlobalScope()) {
        prefix += parentGroup->getFullDescription() + "::";
    }
    return prefix;
}

string Test::getDescription() const {
    return description;
}

void Test::writeBytes(BytesConsumer& consumer) const {
    consumer
        << parentGroup->getIndex()
        << index
        << line
        << file.size()
        << file
        << description.size()
        << description
        << isPassed()
        << getFailureMessage().size()
        << getFailureMessage();
}

JSON Test::toJSON() const {
    JSON report = {
        {"description", getDescription()},
        {"file", getFilename()},
        {"line", getLine()},
        {"executed", isExecuted()},
        {"index", getIndex()}
    };
    if (isExecuted()) {
        report["passed"] = isPassed();
        if (!isPassed()) {
            report["failureMessage"] = getFailureMessage();
        }
    }
    return report;
}

void Test::loadFromJSON(const JSON& json) {
    description = unescapeCharacters(json.get("description").operator std::string());
    file = unescapeCharacters(json.get("file").operator std::string());
    line = json.get("line").operator int();
    executed = json.get("executed").operator bool();
    if (executed && !json.get("passed").operator bool()) {
        delete failure;
        failure = nullptr;
        setFailure(unescapeCharacters(json.get("failureMessage").operator std::string()));
    }
}

Group* Test::getParentGroup() const {
    return parentGroup;
}

}
