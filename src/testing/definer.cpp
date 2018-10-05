#include "testing/definer.hpp"
#include "testing/driver.hpp"
#include "testing/testing.hpp"

using namespace std;


namespace kktest {
namespace __internal {

Definer::Definer(string _file, int _line): file(move(_file)), line(_line) {}

void TestDefiner::operator()(string description, const function<void()>& func) {
    TestingDriver::getGlobalDriver()->addTest(
            new Test(move(description), file, line), func
    );
}

void GroupDefiner::operator()(string description,
                              const function<void()>& func) {
    TestingDriver::getGlobalDriver()->addGroup(
            new Group(move(description), file, line), func
    );
}

void SetUpDefiner::operator()(const function<void()>& func) {
    TestingDriver::getGlobalDriver()->addSetUp(func);
}

void TearDownDefiner::operator()(const function<void()>& func) {
    TestingDriver::getGlobalDriver()->addTearDown(func);
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

} // namespace __internal
} // namespace kktest
