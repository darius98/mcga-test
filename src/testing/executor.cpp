#include "testing/executor.hpp"
#include "testing/driver.hpp"

using namespace std;


namespace kktest {

void Executor::executeSetUps(const vector<Group*>& groups, Test* test) {
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
}

void Executor::executeTest(Test *test, Executable func) {
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
}

void Executor::executeTearDowns(const vector<Group*>& groups, Test* test) {
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

}
