#include <iostream>

#include "test_logger.hpp"

using namespace std;

namespace kktest {

bool TestLogger::AscendingByTestIndex::operator()(Test* a, Test* b) {
    return a->getIndex() < b->getIndex();
}

TestLogger::TestLogger(ostream& _stream, bool _singleTest):
        stream(_stream), singleTest(_singleTest) {}

void TestLogger::logTest(Test *test) const {
    stream << test->getFullDescription()
           << ": "
           << (test->isFailed() ? "FAILED" : "PASSED")
           << "\n";
    if (test->isFailed()) {
        stream << "\t" << test->getFailureMessage() << "\n";
    }
}

void TestLogger::enqueueTestForLogging(Test* test) {
    if (singleTest) {
        if (test->isFailed()) {
            stream << test->getFailureMessage();
        }
        return;
    }
    testsQueue.insert(test);
    while (!testsQueue.empty() &&
                (*testsQueue.begin())->getIndex() == testsLogged + 1) {
        logTest(*testsQueue.begin());
        testsQueue.erase(testsQueue.begin());
        testsLogged += 1;
    }
}

}
