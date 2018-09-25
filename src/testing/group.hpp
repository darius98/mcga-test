#ifndef RUNTIME_TESTING_TESTING_GROUP_H_
#define RUNTIME_TESTING_TESTING_GROUP_H_

#include <functional>
#include <ostream>
#include <string>
#include <vector>

#include "test.hpp"


namespace runtime_testing {

struct Group {
    std::string description;

    bool hasSetUp = false;
    std::function<void()> setUpFunc;

    bool hasTearDown = false;
    std::function<void()> tearDownFunc;

    std::vector<Group*> subGroups;
    std::vector<Test*> tests;

    explicit Group(const std::string& description="");

    void generateTestReport(std::ostream& report,
                            const std::string& currentGroupFullName);

    int getNumFailedTests();
};

}

#endif
