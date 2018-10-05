#include "testing/definer.hpp"
#include "testing/driver.hpp"

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

} // namespace __internal
} // namespace kktest
