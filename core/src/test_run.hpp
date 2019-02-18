#ifndef KKTEST_CORE_SRC_TEST_RUN_HPP_
#define KKTEST_CORE_SRC_TEST_RUN_HPP_

#include <string>

#include "common/interproc/src/message.hpp"
#include "core/src/test.hpp"

namespace kktest {

class TestRun {
 public:
    enum MessageStatus: std::uint8_t {
        FINISHED_SUCCESSFULLY = 0,
        CONFIGURATION_ERROR = 1
    };

    // When we create a test run, we no longer need the test.
    // So we always move it.
    TestRun(Test&& test, std::string failure);
    TestRun(Test&& test, interproc::Message& message);
    TestRun(Test&& test, double timeTicks, bool passed, std::string failure);

    const Test& getTest() const;

    std::string getTestDescription() const;

    int getTestIndex() const;

    bool isTestOptional() const;

    bool isPassed() const;

    std::string getFailure() const;

    interproc::Message toMessage() const;

 private:
    Test test;
    double timeTicks;
    bool passed;
    std::string failure;
};

}

#endif
