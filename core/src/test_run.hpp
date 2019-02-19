#ifndef KKTEST_CORE_SRC_TEST_RUN_HPP_
#define KKTEST_CORE_SRC_TEST_RUN_HPP_

#include <string>

#include "core/src/test.hpp"

namespace kktest {

struct ExecutionInfo {
    double timeTicks;
    bool passed;
    std::string failure;

    void fail(const std::string& _failure);
};

class TestRun {
 public:
    // When we create a test run, we no longer need the test.
    // So we always move it.
    TestRun(Test&& test, std::string failure);
    TestRun(Test&& test, ExecutionInfo&& info);

    Group* getGroup() const;

    int getGroupIndex() const;

    std::string getTestDescription() const;

    int getTestIndex() const;

    bool isTestOptional() const;

    bool isPassed() const;

    std::string getFailure() const;

 private:
    Test test;
    ExecutionInfo info;
};

}

#endif
