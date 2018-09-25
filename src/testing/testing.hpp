#ifndef RUNTIME_TESTING_TESTING_TESTING_H_
#define RUNTIME_TESTING_TESTING_TESTING_H_

#include <functional>
#include <string>


namespace runtime_testing {

void setUp(const std::function<void()>& func);

void tearDown(const std::function<void()>& func);

void test(const std::string& description, const std::function<void()>& func);

void group(const std::string& description, const std::function<void()>& func);

bool isDuringTest();

int numFailedTests();

int getTestReport(std::ostream& report);

}

#endif
