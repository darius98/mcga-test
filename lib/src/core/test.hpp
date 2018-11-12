#ifndef KKTEST_CORE_TEST_H_
#define KKTEST_CORE_TEST_H_

#include <string>

#include <core/config.hpp>


namespace kktest {

class Group;

class Test {
public:
    Test(const TestConfig& _config, Group* _parentGroup, int _index);

    const TestConfig& getConfig() const;

    bool isTopLevel() const;

    int getIndex() const;

    bool isExecuted() const;

    bool isFailed() const;

    bool isPassed() const;

    double getExecutionTimeTicks() const;

    std::string getFailureMessage() const;

    std::string getDescriptionPrefix() const;

    int getGroupIndex() const;

    Group* getGroup() const;

private:
    void setExecuted(double _executionTimeTicks, bool _passed=true, std::string _failureMessage="");

    TestConfig config;

    Group* parentGroup;
    int index;

    bool executed = false;
    bool passed = true;
    std::string failureMessage = "";
    double executionTimeTicks = -1.0;

friend class Executor;
};

}

#endif
