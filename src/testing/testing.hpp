#ifndef RUNTIME_TESTING_TESTING_TESTING_H_
#define RUNTIME_TESTING_TESTING_TESTING_H_

#include <functional>
#include <string>


namespace runtime_testing {

void setUp(const std::function<void()>& setUpFunc);

void tearDown(const std::function<void()>& tearDownFunc);

void test(
        const std::string& description,
        const std::function<void()>& testFunc);

void group(
        const std::string& description,
        const std::function<void()>& groupFunc);

bool isDuringTest();

int numFailedTests();

int getTestReport();

}

#endif
