#ifndef KKTEST_CORE_SRC_TEST_CASE_REGISTRY_HPP_
#define KKTEST_CORE_SRC_TEST_CASE_REGISTRY_HPP_

#include <utility>
#include <vector>

#include <kktest_common/string.hpp>

namespace kktest {

class TestCaseRegistry {
 public:
    typedef void (*TestCase)();

    static void add(TestCase testCase, const char* name);

    static std::vector<std::pair<TestCase, String>> all();

    static void clean();

 private:
    static std::vector<std::pair<TestCase, String>>* testCases;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_TEST_CASE_REGISTRY_HPP_
