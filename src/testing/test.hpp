#ifndef KKTEST_TESTING_TEST_H_
#define KKTEST_TESTING_TEST_H_

#include <string>

#include <JSON>

#include "testing/expectation_failed.hpp"


namespace kktest {

struct Test {
    explicit Test(std::string description, std::string file="", int line=0);

    ~Test();

    bool isFailed() const;

    bool isPassed() const;

    std::string getFailureMessage() const;

    void setFailure(const std::string& message);

    void setFailure(const ExpectationFailed& f);

    autojson::JSON generateReport() const;

    std::string description;

    std::string file;
    int line;
private:
    ExpectationFailed* failure = nullptr;
};

}

#endif
