#ifndef KKTEST_TESTING_TEST_LOGGER_H_
#define KKTEST_TESTING_TEST_LOGGER_H_

#include <ostream>
#include <set>

#include "test.hpp"


namespace kktest {

class TestLogger {
public:
    explicit TestLogger(std::ostream& _stream, bool _singleTest);

    void enqueueTestForLogging(Test* test);
private:
    void logTest(Test* test) const;

    struct AscendingByTestIndex {
        bool operator()(Test* a, Test* b);
    };

    std::ostream& stream;

    bool singleTest;
    int testsLogged = 0;
    std::set<Test*, AscendingByTestIndex> testsQueue;
};

}

#endif
