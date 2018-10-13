#ifndef KKTEST_TESTING_TEST_H_
#define KKTEST_TESTING_TEST_H_

#include <string>

#include <JSON>

#include "expectation_failed.hpp"


namespace kktest {

class Group;

class Test {
private:
    static int globalTestIndex;

public:
    Test(std::string _description,
         std::string _file,
         int _line,
         Group* _parentGroup);

    ~Test();

    int getIndex() const;

    bool isFailed() const;

    bool isPassed() const;

    std::string getFailureMessage() const;

    void setFailure(const std::string& message);

    void setFailure(const ExpectationFailed& f);

    std::string getFullDescription() const;

    autojson::JSON generateReport() const;

    void updateGroups() const;
private:
    Group* parentGroup = nullptr;
    int index;

    std::string description;

    std::string file;
    int line;

    ExpectationFailed* failure = nullptr;
};

}

#endif
