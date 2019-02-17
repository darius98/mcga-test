#ifndef KKTEST_CORE_SRC_TEST_CASE_REGISTRY_HPP_
#define KKTEST_CORE_SRC_TEST_CASE_REGISTRY_HPP_

#include <utility>
#include <string>
#include <vector>

namespace kktest {

class TestCaseRegistry {
 public:
    typedef void (*TestCase)();

    static void add(TestCase testCase, const char* name);

    static std::vector<std::pair<TestCase, std::string>> all();

    static void clean();

 private:
    static std::vector<std::pair<TestCase, std::string>>* testCases;
};

}

#endif
