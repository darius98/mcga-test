#include <core/definers.hpp>

#include "driver.hpp"
#include "errors.hpp"

using namespace std;


namespace kktest {

Definer::Definer(string _file, int _line): file(move(_file)), line(_line) {}

void TestDefiner::operator()(string description, Executable func) {
    (*this)(kkTestConfig(_.description = description), func);
}

void TestDefiner::operator()(const TestConfig& config, Executable func) {
    TestConfig fullConfig(config);
    fullConfig.file = file;
    fullConfig.line = line;
    TestingDriver::getInstance()->addTest(fullConfig, func);
}

void GroupDefiner::operator()(string description, Executable func) {
    (*this)(kkGroupConfig(_.description = description), func);
}

void GroupDefiner::operator()(const GroupConfig& config, Executable func) {
    GroupConfig fullConfig(config);
    fullConfig.file = file;
    fullConfig.line = line;
    TestingDriver::getInstance()->addGroup(fullConfig, func);
}

void SetUpDefiner::operator()(Executable func) {
    TestingDriver::getInstance()->addSetUp(func, file, line);
}

void TearDownDefiner::operator()(Executable func) {
    TestingDriver::getInstance()->addTearDown(func, file, line);
}

void ExpectDefiner::operator()(const bool& exprResult, const string& expr) {
    if (!exprResult) {
        throwExpectationFailed(file + ":" + to_string(line) + ": " + expr);
    }
}

void ExpectDefiner::throwExpectationFailed(const string& message) {
    throw ExpectationFailed(message);
}

}
