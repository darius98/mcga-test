#ifndef KKTEST_CORE_CORE_H_
#define KKTEST_CORE_CORE_H_

namespace kktest {

int testMain(int argc, char** argv);

}

/**
 * Implement this function in each test case target.
 */
void testCase();

#endif
