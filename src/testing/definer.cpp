#include "driver.hpp"
#include "definer.hpp"

using namespace std;


namespace runtime_testing {
namespace __internal {

TestDefiner::TestDefiner(string _file, int _line):
        file(move(_file)), line(_line) {}

void TestDefiner::operator()(string description,
                             const function<void()>& func) {
    TestingDriver::getOrCreateGlobalDriver()->addTest(
            new Test(move(description), file, line), func
    );
}

GroupDefiner::GroupDefiner(string _file, int _line):
        file(move(_file)), line(_line) {}

void GroupDefiner::operator()(string description,
                              const function<void()>& func) {
    TestingDriver::getOrCreateGlobalDriver()->addGroup(
            new Group(move(description), file, line), func
    );
}

} // namespace __internal
} // namespace runtime_testing
