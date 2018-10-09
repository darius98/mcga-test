#include <JSON>
#include <EasyFlags.hpp>

#include "executor.hpp"
#include "driver.hpp"

using namespace autojson;
using namespace std;

AddArgument(int, argumentTestIndex).Name("single-test").DefaultValue(0);
AddArgument(int, flagDebug).Name("debug").DefaultValue(0).ImplicitValue(1);
AddArgument(string, boxId).Name("box-id").DefaultValue("0");


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
        executeSimple(groups, test, func);
        if (test->isFailed()) {
            cout << test->getFailureMessage();
        }
        exit(test->isFailed());
    }
    if (!flagDebug) {
        executeLocked(test, testIndex);
    } else {
        executeSimple(groups, test, func);
    }
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
                              "setUp of group '" + g->getFullDescription()
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "setUp of group '" + g->getFullDescription() + "'.";
            }
            if (failed) {
                test->setFailure(failMessage);
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
        test->setFailure(failure);
    } catch(const exception& e) {
        test->setFailure(
            "An exception was thrown during test: " + string(e.what())
        );
    } catch(...) {
        test->setFailure("A non-exception object was thrown during test");
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
                              "tearDown of group '" + g->getFullDescription()
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "tearDown of group '" + g->getFullDescription() + "'.";
            }
            if (failed) {
                test->setFailure(failMessage);
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
            group->getFullDescription() +
            "': " +
            e.what());
    } catch(...) {
        throw runtime_error(
            "A non-exception object was thrown inside group'" +
            group->getFullDescription() + "'");
    }
}

void Executor::executeLocked(Test *test, int testIndex) {
    string boxDir = "/tmp/box/" + boxId + "/box/";

    if (!copiedBinary) {
        system(("box --init; cp " + name + " " + boxDir + "test").c_str());
        copiedBinary = true;
    }

    string processName = "box --run --meta=" + boxId + " --box-id=" + boxId;
    processName += " -- ./test --enable-logging=0 --single-test=";
    processName += to_string(testIndex);

    FILE* pipe = popen(processName.c_str(), "r");
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    string testOutput;
    char buffer[32];
    while (!feof(pipe)) {
        if (fgets(buffer, 32, pipe) != nullptr) {
            testOutput += buffer;
        }
    }

    JSON runStats = JSON::readFromFile(boxId);
    system(("rm " + boxId).c_str());
    if (pclose(pipe) != 0 || (int)runStats["exitCode"] != 0) {
        test->setFailure(testOutput);
    }
}

void Executor::executeSimple(const vector<Group*>& groups,
                             Test* test,
                             Executable func) {
    executeSetUps(groups, test);
    executeTest(test, func);
    executeTearDowns(groups, test);
}

}
