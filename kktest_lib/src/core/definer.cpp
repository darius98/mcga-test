#include "definer.hpp"
#include "driver.hpp"

using namespace std;


namespace kktest {

Definer::Definer(string _file, int _line): file(move(_file)), line(_line) {}

void TestDefiner::operator()(string description, Executable func) {
    TestingDriver::getInstance()->addTest(move(description), file, line, func);
}

void GroupDefiner::operator()(string description, Executable func) {
    TestingDriver::getInstance()->addGroup(move(description), file, line, func);
}

void SetUpDefiner::operator()(Executable func) {
    TestingDriver::getInstance()->addSetUp(func);
}

void TearDownDefiner::operator()(Executable func) {
    TestingDriver::getInstance()->addTearDown(func);
}

void ExpectDefiner::operator()(const bool& exprResult, const string& expr) {
    checkDuringTest();
    if (!exprResult) {
        throw ExpectationFailed(file + ":" + to_string(line) + ": " + expr);
    }
}

void ExpectDefiner::throwExpectationFailed(const string& message) {
    throw ExpectationFailed(message);
}

void ExpectDefiner::checkDuringTest() {
    if (TestingDriver::instance == nullptr ||
        !TestingDriver::instance->executor->isDuringTest()) {
        throw runtime_error(
            file + ":" + to_string(line) + ": "
            "'expect' can only be called inside tests!"
        );
    }
}

}
