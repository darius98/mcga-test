#include "core/src/test_case_registry.hpp"

#include <cstdlib>
#include <cstring>

#include <utility>

using namespace std;

namespace kktest {

vector<pair<TestCaseRegistry::TestCase, String>>* TestCaseRegistry::testCases;

void TestCaseRegistry::add(TestCase testCase, const char* name) {
    if (testCases == nullptr) {
        testCases = new vector<pair<TestCase, String>>();
    }
    testCases->emplace_back(testCase, name);
}

auto TestCaseRegistry::all() -> vector<pair<TestCase, String>> {
    if (testCases == nullptr) {
        return {};
    }
    return *testCases;
}

void TestCaseRegistry::clean() {
    delete testCases;
}

}
