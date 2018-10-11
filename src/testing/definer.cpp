#include "definer.hpp"
#include "driver.hpp"

using namespace std;


namespace kktest {

Definer::Definer(string _file, int _line): file(move(_file)), line(_line) {}

TestingDriver* Definer::getDriver() const {
    return TestingDriver::getGlobalDriver();
}

void TestDefiner::operator()(string description, Executable func) {
    getDriver()->addTest(move(description), file, line, func);
}

void GroupDefiner::operator()(string description, Executable func) {
    getDriver()->addGroup(move(description), file, line, func);
}

void SetUpDefiner::operator()(Executable func) {
    getDriver()->addSetUp(func);
}

void TearDownDefiner::operator()(Executable func) {
    getDriver()->addTearDown(func);
}

void ExpectDefiner::operator()(const bool& exprResult, const string& expr) {
    if (!TestingDriver::isDuringTest()) {
        throw runtime_error(
            file + ":" + to_string(line) + ": "
            "'expect' can only be called inside tests!"
        );
    }
    if (!exprResult) {
        throw ExpectationFailed(file + ":" + to_string(line) + ": " + expr);
    }
}

}
