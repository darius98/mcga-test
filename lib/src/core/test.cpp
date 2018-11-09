#include "group.hpp"
#include "test.hpp"

using namespace std;


namespace kktest {

Test::Test(string _description,
           string _file,
           int _line,
           Group* _parentGroup,
           int _index):
        description(move(_description)),
        file(move(_file)),
        line(_line),
        parentGroup(_parentGroup),
        index(_index) {}

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
    if (!failure) {
        return "";
    }
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

Group* Test::getParentGroup() const {
    return parentGroup;
}

}
