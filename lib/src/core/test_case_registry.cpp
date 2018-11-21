#include <cstdlib>
#include <cstring>

#include "test_case_registry.hpp"

using namespace std;

namespace kktest {

TestCaseRegistry::TestCase* TestCaseRegistry::testCases = nullptr;
int TestCaseRegistry::numTestCases = 0;
int TestCaseRegistry::testCasesCapacity = 0;

void TestCaseRegistry::add(TestCase testCase) {
    if (numTestCases == 0) {
        testCases = (TestCase*)malloc(sizeof(TestCase));
        testCasesCapacity = 1;
    }
    if (numTestCases == testCasesCapacity) {
        auto newTestCases = (TestCase*)malloc(sizeof(TestCase) * (2 * testCasesCapacity));
        memcpy(newTestCases, testCases, numTestCases * sizeof(TestCase));
        free(testCases);
        testCases = newTestCases;
    }
    testCases[numTestCases++] = testCase;
}

vector<TestCaseRegistry::TestCase> TestCaseRegistry::all() {
    return vector<TestCase>(testCases, testCases + numTestCases);
}

void TestCaseRegistry::clean() {
    if (numTestCases != 0) {
        free(testCases);
    }
}

}
