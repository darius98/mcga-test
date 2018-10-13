#include <iostream>

#include "test_logger.hpp"

using namespace std;

namespace kktest {

bool TestLogger::AscendingByTestIndex::operator()(Test* const& a,
                                                  Test* const& b) {
    return a->getIndex() < b->getIndex();
}

void TestLogger::enqueueTestForLogging(Test* test) {
    testsQueue.insert(test);
    while (!testsQueue.empty() &&
                (*testsQueue.begin())->getIndex() == testsLogged + 1) {
        Test* testToLog = *testsQueue.begin();
        cout << testToLog->getFullDescription()
             << ": "
             << (testToLog->isFailed() ? "FAILED" : "PASSED")
             << "\n";
        if (testToLog->isFailed()) {
            cout << "\t" << testToLog->getFailureMessage() << "\n";
        }
        testsLogged += 1;
        testsQueue.erase(testsQueue.begin());
    }
}

}
