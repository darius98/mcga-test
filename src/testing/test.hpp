#ifndef KTEST_TESTING_TEST_H_
#define KTEST_TESTING_TEST_H_

#include <ostream>
#include <string>

#include "expect.hpp"


namespace ktest {

struct Test {
    std::string file;
    int line;

    std::string description;

    ExpectationFailed* failure = nullptr;

    explicit Test(std::string description, std::string file="", int line=0);

    ~Test();

    void generateTestReport(std::ostream& report, std::size_t spaces);
};

}

#endif
