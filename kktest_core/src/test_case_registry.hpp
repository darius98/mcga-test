#ifndef KKTEST_CORE_TEST_CASE_REGISTRY_H_
#define KKTEST_CORE_TEST_CASE_REGISTRY_H_

#include <vector>

#include <kktest_impl/types.hpp>

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

}

#endif
