#include "driver.hpp"
#include "definer.hpp"

using namespace std;


namespace ktest {
namespace __internal {

Definer::Definer(string _file, int _line): file(move(_file)), line(_line) {}

void TestDefiner::operator()(string description, const function<void()>& func) {
    TestingDriver::getOrCreateGlobalDriver()->addTest(
            new Test(move(description), file, line), func
    );
}

void GroupDefiner::operator()(string description,
                              const function<void()>& func) {
    TestingDriver::getOrCreateGlobalDriver()->addGroup(
            new Group(move(description), file, line), func
    );
}

void SetUpDefiner::operator()(const function<void()>& func) {
    TestingDriver::getOrCreateGlobalDriver()->addSetUp(func);
}

void TearDownDefiner::operator()(const function<void()>& func) {
    TestingDriver::getOrCreateGlobalDriver()->addTearDown(func);
}

} // namespace __internal
} // namespace ktest
