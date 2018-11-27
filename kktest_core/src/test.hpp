#ifndef KKTEST_CORE_TEST_H_
#define KKTEST_CORE_TEST_H_

#include <string>

#include <kktest_impl/config.hpp>
#include <kktest_impl/info.hpp>

namespace kktest {

class Group;

class Test {
public:
    Test(const TestConfig& _config, Group* _parentGroup, int _index);

    const TestConfig& getConfig() const;

    bool isExecuted() const;

    bool isFailed() const;

    bool isPassed() const;

    double getExecutionTimeTicks() const;

    std::string getFailureMessage() const;

    TestInfo getTestInfo() const;

    Group* getGroup() const;

    void setExecuted(double _executionTimeTicks, bool _passed, std::string _failureMessage);

private:
    TestConfig config;

    Group* parentGroup;
    int index;

    bool executed = false;
    bool passed = true;
    std::string failureMessage = "";
    double executionTimeTicks = -1.0;
};

}

#endif
