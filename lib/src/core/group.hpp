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

    std::string getRenderedFailureMessageOnExceptionInSetUp(const std::string& what) const;

    std::string getRenderedFailureMessageOnNonExceptionInSetUp() const;

    std::string getRenderedFailureMessageOnExceptionInTearDown(const std::string& what) const;

    std::string getRenderedFailureMessageOnNonExceptionInTearDown() const;

private:
    void addSetUp(Executable func, const std::string& file, int line);

    void setUp() const;

    void addTearDown(Executable func, const std::string& file, int line);

    void tearDown() const;

    void markTestStartedExecution();

    void markTestFinishedExecution();

    void markAllTestsStartedExecution(Executable _afterAllTestsCallback);

    GroupConfig config;

    Group* parentGroup;
    int index;

    bool hasSetUp = false;
    CopyableExecutable setUpFunc;
    std::string setUpFile;
    int setUpLine;

    bool hasTearDown = false;
    CopyableExecutable tearDownFunc;
    std::string tearDownFile;
    int tearDownLine;

    int testsStarted = 0;
    int testsFinished = 0;
    bool allTestsStarted = false;
    CopyableExecutable afterAllTestsCallback;

friend class Executor;
friend class TestingDriver;
};

}

#endif
