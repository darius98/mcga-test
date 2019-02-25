#include "kktest/core/include/kktest.hpp"

#include "kktest/core/src/main.hpp"
#include "kktest/extensions/feedback/src/ext.hpp"

using namespace std;

namespace {

vector<kktest::Extension*>* extensions = nullptr;
bool initCalled = false;
bool runCalled = false;

}

namespace kktest {

void init(int argc, char** argv) {
    if (initCalled) {
        throw runtime_error("kktest::init() called twice!");
    }
    initCalled = true;
    extensions = new vector<Extension*>{
        new feedback::FeedbackExtension()
    };
    initialize(argc, argv, extensions);
}

int run(vector<TestCase> tests) {
    if (runCalled) {
        throw runtime_error("kktest::run() called twice!");
    }
    if (!initCalled) {
        throw runtime_error("kktest::run() called before kktest::init()!");
    }
    runCalled = true;
    return runTests(move(tests), extensions);
}

int initAndRun(int argc, char **argv, vector<TestCase> tests) {
    init(argc, argv);
    return run(move(tests));
}

}
