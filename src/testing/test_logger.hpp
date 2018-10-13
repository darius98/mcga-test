#ifndef KKTEST_TESTING_TEST_LOGGER_H_
#define KKTEST_TESTING_TEST_LOGGER_H_

#include <set>

#include "test.hpp"


namespace kktest {

class TestLogger {
public:
    void enqueueTestForLogging(Test* test);
private:
    struct AscendingByTestIndex {
        bool operator()(Test* const& a, Test* const& b);
    };

    int testsLogged = 0;
    std::set<Test*, AscendingByTestIndex> testsQueue;
};

}

#endif
