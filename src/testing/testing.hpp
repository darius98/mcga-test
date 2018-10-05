#ifndef KKTEST_TESTING_TESTING_H_
#define KKTEST_TESTING_TESTING_H_


namespace kktest {

void initializeTestingDriver(int argc, char** argv);

bool isDuringTest();

int numFailedTests();

void destroyTestingDriver();

int finalizeTesting();

}

#endif
