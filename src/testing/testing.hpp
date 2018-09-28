#ifndef RUNTIME_TESTING_TESTING_TESTING_H_
#define RUNTIME_TESTING_TESTING_TESTING_H_

#include <functional>
#include <iostream>
#include <string>

#include "definer.hpp"


namespace runtime_testing {

void initializeTestingDriver(std::ostream& log=std::cerr);

void setUp(const std::function<void()>& func);

void tearDown(const std::function<void()>& func);

bool isDuringTest();

int numFailedTests();

int writeTestSuiteReport(std::ostream &report);

void destroyTestingDriver();

int finalizeTesting(const std::string& reportFileName="report.json");

}

#define test runtime_testing::__internal::TestDefiner(__FILENAME__, __LINE__)

#define group runtime_testing::__internal::GroupDefiner(__FILENAME__, __LINE__)

#endif
