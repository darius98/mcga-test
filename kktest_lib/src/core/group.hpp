#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <functional>
#include <string>
#include <vector>

#include <JSON>

#include <kktest_executable.hpp>
#include "test.hpp"


namespace kktest {

class Group: public MessageSerializable {
private:
    static int globalGroupIndex;

public:
    Group(std::string _description,
          std::string _file,
          int _line,
          Group* _parentGroup);

    ~Group();

    int getNumFailedTests() const;
    int getNumTests() const;

    bool isGlobalScope() const;

    Group* addSubGroup(std::string description, std::string file, int line);

    Test* addTest(std::string description, std::string file, int line);

    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

    std::string getFullDescription() const;

    void writeBytes(BytesConsumer& consumer) const override;

    autojson::JSON toJSON() const;

    int getIndex() const;

    Group* getParentGroup() const;

private:
    std::vector<Group*> subGroups;
    std::vector<Test*> tests;

    std::string description;
    std::string file;
    int line;

    Group* parentGroup;
    int index;

    bool hasSetUp = false;
    CopyableExecutable setUpFunc;

    bool hasTearDown = false;
    CopyableExecutable tearDownFunc;
};

}

#endif
