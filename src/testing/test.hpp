#ifndef RUNTIME_TESTING_TESTING_TEST_H_
#define RUNTIME_TESTING_TESTING_TEST_H_

#include <functional>
#include <string>


namespace runtime_testing {

bool isDuringTest();

void test(const std::string& description, std::function<void()> testFunc);

void group(const std::string& description, std::function<void()> groupFunc);

void setUp(std::function<void()> setUpFunc);

void tearDown(std::function<void()> tearDownFunc);

int getTestReport();

}

#endif
