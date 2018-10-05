#include "testing/definer.hpp"
#include "testing/driver.hpp"

using namespace std;


namespace kktest {

Definer::Definer(string _file, int _line): file(move(_file)), line(_line) {}

TestingDriver* Definer::getDriver() {
    return TestingDriver::getGlobalDriver();
}

bool Definer::isDuringTest() {
    return TestingDriver::isDuringTest();
}

void TestDefiner::operator()(string description, Executable func) {
    getDriver()->addTest(new Test(move(description), file, line), func);
}

void GroupDefiner::operator()(string description, Executable func) {
    getDriver()->addGroup(new Group(move(description), file, line), func);
}

void SetUpDefiner::operator()(Executable func) {
    getDriver()->addSetUp(func);
}

void TearDownDefiner::operator()(Executable func) {
    getDriver()->addTearDown(func);
}

void BaseExpectDefiner::checkDuringTest() {
    if (!isDuringTest()) {
        throw runtime_error(
            file + ":" + to_string(line) + ": "
            "'expect' can only be called inside tests!"
        );
    }
}

void BaseExpectDefiner::throwExpectationFailed(Description* description) {
    string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}

void ExpectDefiner::operator()(const bool& exprResult, const string& expr) {
    checkDuringTest();
    if (!exprResult) {
        throwExpectationFailed(Description::createForExpect(file, line, expr));
    }
}

} // namespace kktest
