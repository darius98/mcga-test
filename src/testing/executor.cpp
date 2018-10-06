#include <EasyFlags.hpp>

#include "testing/executor.hpp"
#include "testing/driver.hpp"

using namespace std;

AddArgument(int, argumentTestIndex).Name("single-test").DefaultValue(0);


namespace kktest {

Executor::Executor(const string& _name): name(_name) {}

bool Executor::isDuringTest() const {
    return state == ExecutorState::TEST;
}

void Executor::execute(const vector<Group*>& groups,
                       Test* test,
                       Executable func) {
    testIndex += 1;
    if (argumentTestIndex != 0 && testIndex != argumentTestIndex) {
        return;
    }
    if (argumentTestIndex != 0) {
        executeSetUps(groups, test);
        executeTest(test, func);
        executeTearDowns(groups, test);
        if (test->failure) {
            cout << test->failure->getMessage();
        }
        exit(test->failure != nullptr);
    }
    executeLocked(test, testIndex);
}

void Executor::checkIsInactive(const string& methodName) const {
    if (state == ExecutorState::TEST) {
        throw runtime_error("Cannot call " + methodName + " within test!");
    }
    if (state == ExecutorState::SET_UP) {
        throw runtime_error("Cannot call " + methodName + " within setUp!");
    }
    if (state == ExecutorState::TEAR_DOWN) {
        throw runtime_error("Cannot call " + methodName + " within tearDown!");
    }
}

void Executor::executeSetUps(const vector<Group*>& groups, Test* test) {
    state = ExecutorState::SET_UP;
    for (Group* g: groups) {
        if (g->hasSetUp) {
            bool failed;
            string failMessage;
            try {
                g->setUpFunc();
                failed = false;
            } catch(const exception& e) {
                failed = true;
                failMessage = "An exception was thrown during the "
                              "setUp of group '" + g->description
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "setUp of group '" + g->description + "'.";
            }
            if (failed && test->failure == nullptr) {
                test->failure = new ExpectationFailed(failMessage);
            }
        }
    }
    state = ExecutorState::INACTIVE;
}

void Executor::executeTest(Test *test, Executable func) {
    state = ExecutorState::TEST;
    try {
        func();
    } catch(const ExpectationFailed& failure) {
        if (test->failure == nullptr) {
            test->failure = new ExpectationFailed(failure);
        }
    } catch(const exception& e) {
        if (test->failure == nullptr) {
            test->failure = new ExpectationFailed(
                "An exception was thrown during test: " + string(e.what())
            );
        }
    } catch(...) {
        if (test->failure == nullptr) {
            test->failure = new ExpectationFailed(
                "A non-exception object was thrown during test"
            );
        }
    }
    state = ExecutorState::INACTIVE;
}

void Executor::executeTearDowns(const vector<Group*>& groups, Test* test) {
    state = ExecutorState::TEAR_DOWN;
    for (int i = (int)groups.size() - 1; i >= 0; -- i) {
        Group* g = groups[i];
        if (g->hasTearDown) {
            bool failed;
            string failMessage;
            try {
                g->tearDownFunc();
                failed = false;
            } catch(const exception& e) {
                failed = true;
                failMessage = "An exception was thrown during the "
                              "tearDown of group '" + g->description
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "tearDown of group '" + g->description + "'.";
            }
            if (failed && test->failure == nullptr) {
                test->failure = new ExpectationFailed(failMessage);
            }
        }
    }
    state = ExecutorState::INACTIVE;
}

void Executor::executeGroup(Group* group, Executable func) {
    try {
        func();
    } catch(const exception& e) {
        throw runtime_error("An exception was thrown inside group '" +
            group->description +
            "': " +
            e.what());
    } catch(...) {
        throw runtime_error(
            "A non-exception object was thrown inside group'" +
            group->description + "'");
    }
}

void Executor::executeLocked(Test *test, int testIndex) {
    string processName = name;
    processName += " --enable-logging=0";
    processName += " --single-test=" + to_string(testIndex);

    // TODO(darius98): Replace this with alexvelea's isolate
    FILE* pipe = popen(processName.c_str(), "r");
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }

    string result;

    char buffer[32];
    while (!feof(pipe)) {
        if (fgets(buffer, 32, pipe) != nullptr) {
            result += buffer;
        }
    }
    auto rc = pclose(pipe);

    if (rc != EXIT_SUCCESS) { // == 0
        test->failure = new ExpectationFailed(result);
    }
}

}