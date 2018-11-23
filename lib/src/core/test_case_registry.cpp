#include <cstdlib>
#include <cstring>

#include <core/test_case_registry.hpp>

using namespace std;

namespace kktest {

vector<TestCaseRegistry::TestCase>* TestCaseRegistry::testCases = nullptr;

void TestCaseRegistry::add(TestCase testCase) {
    if (testCases == nullptr) {
        testCases = new vector<TestCase>();
    }
    testCases->push_back(testCase);
}

vector<TestCaseRegistry::TestCase> TestCaseRegistry::all() {
    if (testCases == nullptr) {
        return {};
    }
    return *testCases;
}

void TestCaseRegistry::clean() {
    delete testCases;
}

}
