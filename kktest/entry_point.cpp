#include "kktest/core/include/kktest.hpp"

#include <iostream>

#include "kktest/core/src/export.hpp"
#include "kktest/core/src/main.hpp"
#include "kktest/extensions/feedback/src/ext.hpp"

using namespace std;

namespace {

vector<kktest::Extension*>* extensions = nullptr;
bool initCalled = false;
bool runCalled = false;
int runResult = -1;

}

namespace kktest {

KKTEST_EXPORT void init(int argc, char** argv) {
    if (initCalled) {
        cout << "KKTest Warning: kktest::init() was called before!          \n";
        cout << "                Ignoring this call...                      \n";
        return;
    }
    initCalled = true;
    extensions = new vector<Extension*>{
        new feedback::FeedbackExtension()
    };
    initialize(argc, argv, extensions);
}

KKTEST_EXPORT int run(vector<TestCase> tests) {
    if (runCalled) {
        cout << "KKTest Warning: kktest::run() called twice! Returning the  \n";
        cout << "                same result as the first execution.        \n";
        return runResult;
    }
    if (!initCalled) {
        cout << "KKTest Warning: kktest::run() called before kktest::init().\n";
        cout << "                Ignoring command-line flags and using      \n";
        cout << "                default configuration instead.             \n";
        init(0, nullptr);
    }
    runCalled = true;
    runResult = runTests(move(tests), extensions);
    return runResult;
}

}
