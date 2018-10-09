#ifndef KKTEST_TESTING_TEST_H_
#define KKTEST_TESTING_TEST_H_

#include <string>

#include <JSON>

#include "expectation_failed.hpp"


namespace kktest {

struct Group;

struct Test {
    explicit Test(std::string _description, std::string _file, int _line);

    ~Test();

    bool isFailed() const;

    bool isPassed() const;

    std::string getFailureMessage() const;

    void setFailure(const std::string& message);

    void setFailure(const ExpectationFailed& f);

    std::string getFullDescription() const;

    autojson::JSON generateReport() const;

    Group* parentGroup = nullptr;
private:
    std::string description;

    std::string file;
    int line;

    ExpectationFailed* failure = nullptr;
};

}

#endif
