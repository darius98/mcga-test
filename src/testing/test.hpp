#ifndef RUNTIME_TESTING_TESTING_TEST_H_
#define RUNTIME_TESTING_TESTING_TEST_H_

#include <ostream>
#include <string>

#include "expect.hpp"


namespace runtime_testing {

struct Test {
    std::string description;

    ExpectationFailed* failure = nullptr;

    explicit Test(const std::string& description);

    ~Test();

    void generateTestReport(std::ostream& report, std::size_t spaces);
};

}

#endif
