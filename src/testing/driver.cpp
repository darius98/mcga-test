#include <iostream>

#include <EasyFlags.hpp>

#include "box_executor.hpp"
#include "driver.hpp"
#include "smooth_executor.hpp"

using namespace easyflags;
using namespace std;

AddArgument(int, flagQuiet)
    .ArgumentType("0|1 ")
    .Name("quiet")
    .Short("q")
    .Description("Disable STDOUT logging for this test run")
    .DefaultValue(0)
    .ImplicitValue(1);
AddArgument(int, flagBoxed)
    .ArgumentType("0|1 ")
    .Name("boxed")
    .Short("b")
    .Description("Run the tests boxed (requires sudo + box installed)")
    .DefaultValue(0)
    .ImplicitValue(1);
AddArgument(int, argumentTestIndex)
    .ArgumentType("int ")
    .Name("test")
    .Short("t")
    .Description("Index of the test to run (defaults to 0 - run all tests).")
    .DefaultValue(0)
    .ImplicitValue(0);


namespace kktest {

TestingDriver* TestingDriver::instance = nullptr;

TestingDriver* TestingDriver::getInstance() {
    if (instance == nullptr) {
        cout << "Global testing driver not defined. Cannot continue.\n";
        exit(1);
    }
    return instance;
}

void TestingDriver::init(const string &binaryPath) {
    if (instance != nullptr) {
        throw runtime_error("Testing driver cannot be initialized: "
                            "it already exists.");
    }
    instance = new TestingDriver(binaryPath);
}

void TestingDriver::generateTestReport(ostream& report) {
    getInstance()->executor->finalize();
    report << getInstance()->groupStack[0]->generateReport().stringify(0);
}

int TestingDriver::destroy() {
    TestingDriver* driver = getInstance();
    driver->executor->finalize();
    int status = driver->getNumFailedTests();
    delete driver;
    instance = nullptr;
    return status;
}

bool TestingDriver::isDuringTest() {
    return instance != nullptr &&
            instance->executor->isDuringTest();
}

void TestingDriver::addAfterTestHook(Executor::Hook hook) {
    getInstance()->executor->addAfterTestHook(move(hook));
}

TestingDriver::TestingDriver(const string& binaryPath):
        globalScope(new Group("", "", 0, nullptr)),
        groupStack({globalScope}) {
    if (flagBoxed) {
        executor = new BoxExecutor(argumentTestIndex, binaryPath);
    } else {
        executor = new SmoothExecutor(argumentTestIndex);
    }
    executor->addAfterTestHook([](Test* test) {
        test->updateGroups();
    });
    executor->addAfterTestHook([](Test* test) {
        if (test->isFailed() && argumentTestIndex == test->getIndex()) {
            cout << test->getFailureMessage();
        }
    });
    if (!flagQuiet) {
        testLogger = new TestLogger();
        executor->addAfterTestHook([this](Test* test) {
            testLogger->enqueueTestForLogging(test);
        });
    }
}

TestingDriver::~TestingDriver() {
    delete globalScope;
    delete executor;
    delete testLogger;
}

void TestingDriver::addGroup(string description,
                             string file,
                             int line,
                             Executable func) {
    executor->checkIsInactive("group");
    Group* group = groupStack.back()->addSubGroup(
        move(description), move(file), line
    );
    groupStack.push_back(group);

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

    groupStack.pop_back();
}

void TestingDriver::addTest(string description,
                            string file,
                            int line,
                            Executable func) {
    executor->checkIsInactive("test");
    Test* test = groupStack.back()->addTest(
        move(description), move(file), line
    );
    executor->executeTest(groupStack, test, func);
}

void TestingDriver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(func);
}

int TestingDriver::getNumFailedTests() {
    return globalScope->numFailedTests;
}

}
