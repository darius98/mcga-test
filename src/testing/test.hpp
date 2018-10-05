#ifndef KKTEST_TESTING_TEST_H_
#define KKTEST_TESTING_TEST_H_

#include <string>

#include <JSON>

#include "testing/expectation_failed.hpp"


namespace kktest {

struct Test {
    std::string file;
    int line;

    std::string description;

    ExpectationFailed* failure = nullptr;

    explicit Test(std::string description, std::string file="", int line=0);

    ~Test();

    autojson::JSON generateReport() const;
};

}

#endif
