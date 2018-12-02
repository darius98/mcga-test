#include <cstdlib>
#include <cstring>

#include <utility>

#include "test_case_registry.hpp"

using std::pair;
using std::vector;

namespace kktest {

vector<pair<TestCaseRegistry::TestCase, String>>* TestCaseRegistry::testCases = nullptr;

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

}  // namespace kktest
