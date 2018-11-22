#ifndef KKTEST_CORE_TEST_CASE_REGISTRY_H_
#define KKTEST_CORE_TEST_CASE_REGISTRY_H_

#include <vector>

namespace kktest {

class TestCaseRegistry {
public:
    typedef void (*TestCase)();

    static void add(TestCase testCase);

    static std::vector<TestCase> all();

    static void clean();

private:
    static std::vector<TestCase>* testCases;
};

}

#endif
