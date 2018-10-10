#include "executor.hpp"

using namespace std;

namespace kktest {

void Executor::executeTest(const vector<Group*>& groups,
                           Test* test,
                           Executable func) {
    currentTestIndex += 1;
    execute(groups, test, func);
}

int Executor::getCurrentTestIndex() const {
    return currentTestIndex;
}

}
