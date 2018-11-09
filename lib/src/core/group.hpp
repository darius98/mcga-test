#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <string>

#include <core/executable.hpp>
#include "test.hpp"


namespace kktest {

class Group: public MessageSerializable {
public:
    Group(std::string _description,
          std::string _file,
          int _line,
          Group* _parentGroup,
          int _index);

    std::string getDescription() const;

    std::string getFilename() const;

    int getLine() const;

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
    std::string description;
    std::string file;
    int line;

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
