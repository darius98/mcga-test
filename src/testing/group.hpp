#ifndef KKTEST_TESTING_GROUP_H_
#define KKTEST_TESTING_GROUP_H_

#include <functional>
#include <string>
#include <vector>

#include <JSON>

#include "executable.hpp"
#include "test.hpp"


namespace kktest {

struct Group {
    Group(std::string _description,
          std::string _file,
          int _line,
          Group* _parentGroup);

    ~Group();

    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

    std::string getFullDescription() const;

    autojson::JSON generateReport() const;

    Group* parentGroup = nullptr;
    std::vector<Group*> subGroups;
    std::vector<Test*> tests;

    int numFailedTests = 0;
    int numTests = 0;
private:
    std::string file;
    int line;

    std::string description;

    bool hasSetUp = false;
    CopyableExecutable setUpFunc;

    bool hasTearDown = false;
    CopyableExecutable tearDownFunc;
};

}

#endif
