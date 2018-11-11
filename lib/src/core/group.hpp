#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <string>

#include <core/executable.hpp>
#include "test.hpp"


namespace kktest {

class Group {
public:
    Group(const GroupConfig& _config, Group* _parentGroup, int _index);

    const GroupConfig& getConfig() const;

    bool isTopLevel() const;

    std::string getFullDescription() const;

    int getIndex() const;

    int getParentGroupIndex() const;

    Group* getParentGroup() const;

private:
    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

    void markTestStartedExecution();

    void markTestFinishedExecution();

    void markAllTestsStartedExecution(Executable _afterAllTestsCallback);

    GroupConfig config;

    Group* parentGroup;
    int index;

    bool hasSetUp = false;
    CopyableExecutable setUpFunc;

    bool hasTearDown = false;
    CopyableExecutable tearDownFunc;

    int testsStarted = 0;
    int testsFinished = 0;
    bool allTestsStarted = false;
    CopyableExecutable afterAllTestsCallback;

friend class Executor;
friend class TestingDriver;
};

}

#endif
