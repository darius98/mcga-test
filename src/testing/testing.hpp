#ifndef KKTEST_TESTING_TESTING_H_
#define KKTEST_TESTING_TESTING_H_

#include <iostream>
#include <string>


namespace kktest {

void initializeTestingDriver(int argc, char** argv);

bool isDuringTest();

int numFailedTests();

void writeTestSuiteReport(std::ostream &report);

void destroyTestingDriver();

int finalizeTesting();

}

#endif
