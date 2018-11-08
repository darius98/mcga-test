#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <functional>
#include <string>
#include <vector>

#include <core/executable.hpp>
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

    ~Group() override;

    std::string getDescription() const;

    std::string getFilename() const;

    int getLine() const;

    bool isGlobalScope() const;

    Test* addTest(std::string description,
                  std::string file,
                  int line,
                  int index);

    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

    std::string getFullDescription() const;

    void writeBytes(BytesConsumer& consumer) const override;

    int getIndex() const;

    Group* getParentGroup() const;

private:
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
