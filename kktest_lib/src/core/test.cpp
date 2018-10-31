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

void Test::setExecuted() {
    if (isExecuted()) {
        throw runtime_error("Test::setExecuted called twice on the same test!");
    }
    executed = true;
    Group* parent = parentGroup;
    while (parent != nullptr) {
        parent->updateWithExecutedTest(this);
        parent = parent->getParentGroup();
    }
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

size_t Test::numBytes() const {
    return sizeof(int)                /* parentIndex */
         + sizeof(int)                /* index */
         + sizeof(int)                /* line */
         + sizeof(size_t)             /* file.size() */
         + file.size()                /* file */
         + sizeof(size_t)             /* description.size() */
         + description.size()         /* description */
         + sizeof(bool)               /* isPassed() */
         + sizeof(size_t)             /* failure message size */
         + getFailureMessage().size() /* failure message */
         ;
}

void Test::writeBytes(uint8_t* dst) const {
    int cursor = 0;
    int parentIndex = parentGroup->getIndex();
    size_t fileSize = file.size();
    size_t descriptionSize = description.size();
    bool passed = isPassed();
    string failureMessage = getFailureMessage();
    size_t failureMessageSize = failureMessage.size();
    memcpy(dst + cursor, &parentIndex, sizeof(int));
    cursor += sizeof(int);
    memcpy(dst + cursor, &index, sizeof(int));
    cursor += sizeof(int);
    memcpy(dst + cursor, &line, sizeof(int));
    cursor += sizeof(int);
    memcpy(dst + cursor, &fileSize, sizeof(size_t));
    cursor += sizeof(size_t);
    memcpy(dst + cursor, file.c_str(), fileSize);
    cursor += fileSize;
    memcpy(dst + cursor, &descriptionSize, sizeof(size_t));
    cursor += sizeof(size_t);
    memcpy(dst + cursor, description.c_str(), descriptionSize);
    cursor += descriptionSize;
    memcpy(dst + cursor, &passed, sizeof(bool));
    cursor += sizeof(bool);
    memcpy(dst + cursor, &failureMessageSize, sizeof(size_t));
    cursor += sizeof(size_t);
    memcpy(dst + cursor, failureMessage.c_str(), failureMessageSize);
}

JSON Test::toJSON() const {
    JSON report = {
        {"description", description},
        {"file", file},
        {"line", line},
        {"executed", isExecuted()}
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
    description = unescapeCharacters(json.get("description").operator string());
    file = unescapeCharacters(json.get("file").operator string());
    line = json.get("line").operator int();
    executed = json.get("executed").operator bool();
    if (executed && json.get("passed").operator bool() == false) {
        delete failure;
        failure = nullptr;
        setFailure(unescapeCharacters(json.get("failureMessage").operator string()));
    }
}

Group* Test::getParentGroup() const {
    return parentGroup;
}

}
