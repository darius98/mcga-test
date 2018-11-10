#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <string>

#include <core/executable.hpp>
#include "test.hpp"


namespace kktest {

class Group: public MessageSerializable {
public:
    Group(const GroupConfig& _config, Group* _parentGroup, int _index);

    const GroupConfig& getConfig() const;

    bool isGlobalScope() const;

    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

    std::string getFullDescription() const;

    void writeBytes(BytesConsumer& consumer) const override;

    int getIndex() const;

    Group* getParentGroup() const;

    void markTestStartedExecution();

    void markTestFinishedExecution();

    void markAllTestsStartedExecution(Executable _afterAllTestsCallback);

private:
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
};

}

#endif
