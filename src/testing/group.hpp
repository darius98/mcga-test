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
    explicit Group(std::string description="", std::string file="", int line=0);

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
