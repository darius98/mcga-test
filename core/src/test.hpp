#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include "core/include/kktest.hpp"
#include "core/src/group.hpp"

namespace kktest {

class Test : private TestConfig {
 public:
    Test(TestConfig&& _config, Group* _group, int _index);

    Test(Test&& other) noexcept = default;
    Test& operator=(Test&& other) noexcept = default;

    Test(const Test& other) = delete;
    Test& operator=(const Test& other) = delete;

    std::string getDescription() const;

    bool isOptional() const;

    double getTimeTicksLimit() const;

    Group* getGroup() const;

    int getIndex() const;

 private:
    Group* group;
    int index;
};

class ExecutedTest : public Test {
 public:
    struct Info {
        double timeTicks;
        bool passed;
        std::string failure;

        Info();
        explicit Info(std::string&& _failure);
        Info(Info&& other) noexcept;
        Info& operator=(Info&& other) noexcept;

        Info(const Info& other) = delete;

        void fail(const std::string& _failure);
    };

    // When we create a test run, we no longer need the test.
    // So we always move it.
    ExecutedTest(Test&& test, std::string&& failure);
    ExecutedTest(Test&& test, Info&& info);

    bool isPassed() const;

    std::string getFailure() const;

 private:
    Info info;
};

}

#endif
