#include <iostream>

#include "test_logger.hpp"

using namespace std;

namespace kktest {

bool TestLogger::AscendingByTestIndex::operator()(Test* const& a,
                                                  Test* const& b) {
    return a->getIndex() < b->getIndex();
}

void TestLogger::enqueueTestForLogging(Test* test) {
    loggingQueue.insert(test);
    while (!loggingQueue.empty() &&
                (*loggingQueue.begin())->getIndex() == testsLogged + 1) {
        Test* test = *loggingQueue.begin();
        cout << test->getFullDescription()
             << ": "
             << (test->isFailed() ? "FAILED" : "PASSED")
             << "\n";
        if (test->isFailed()) {
            cout << "\t" << test->getFailureMessage() << "\n";
        }
        testsLogged += 1;
        loggingQueue.erase(loggingQueue.begin());
    }
}

}
