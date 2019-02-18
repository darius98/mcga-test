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

    explicit TestRun(Test* _test, std::string _failure);
    explicit TestRun(Test* _test, interproc::Message& message);
    TestRun(Test* _test, double _timeTicks, bool _passed, std::string _failure);

    Test* getTest() const;

    bool isPassed() const;

    std::string getFailure() const;

    interproc::Message toMessage() const;

 private:
    Test* test;
    double timeTicks;
    bool passed;
    std::string failure;
};

}

#endif
