#ifndef KKTEST_TESTING_GROUP_H_
#define KKTEST_TESTING_GROUP_H_

#include <functional>
#include <string>
#include <vector>

#include <JSON>

#include "testing/executable.hpp"
#include "testing/test.hpp"


namespace kktest {

struct Group {
    std::string file;
    int line;

    std::string description;

    bool hasSetUp = false;
    CopyableExecutable setUpFunc;

    bool hasTearDown = false;
    CopyableExecutable tearDownFunc;

    std::vector<Group*> subGroups;
    std::vector<Test*> tests;

    int numFailedTests = 0;
    int numTests = 0;

    explicit Group(std::string description="", std::string file="", int line=0);

    ~Group();

    autojson::JSON generateReport() const;
};

}

#endif
