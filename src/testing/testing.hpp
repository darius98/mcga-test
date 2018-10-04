#ifndef KKTEST_TESTING_TESTING_H_
#define KKTEST_TESTING_TESTING_H_

#include <iostream>
#include <string>


namespace kktest {

void initializeTestingDriver(std::ostream& log=std::cout);

bool isDuringTest();

int numFailedTests();

int writeTestSuiteReport(std::ostream &report);

void destroyTestingDriver();

int finalizeTesting(const std::string& reportFileName="report.json");

}

#endif
