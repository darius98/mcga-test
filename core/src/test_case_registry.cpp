#include "core/src/test_case_registry.hpp"

using namespace std;

namespace kktest {

vector<pair<TestCaseRegistry::TestCase, string>>* TestCaseRegistry::testCases;

void TestCaseRegistry::add(TestCase testCase, const char* name) {
    if (testCases == nullptr) {
        testCases = new vector<pair<TestCase, string>>();
    }
    testCases->emplace_back(testCase, name);
}

vector<pair<TestCaseRegistry::TestCase, string>> TestCaseRegistry::all() {
    if (testCases == nullptr) {
        return {};
    }
    return *testCases;
}

void TestCaseRegistry::clean() {
    delete testCases;
}

}
