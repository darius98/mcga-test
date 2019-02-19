#ifndef KKTEST_CORE_INCLUDE_KKTEST_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_HPP_

#include <functional>
#include <string>

namespace kktest {

struct TestConfig {
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;

    TestConfig();
    TestConfig(std::string _description); // NOLINT(google-explicit-constructor)
    TestConfig(const char* _description); // NOLINT(google-explicit-constructor)

    TestConfig& setDescription(std::string _description);
    TestConfig& setOptional(bool _optional);
    TestConfig& setTimeTicksLimit(double _timeTicksLimit);
};

struct GroupConfig {
    std::string description = "-";

    GroupConfig();
    GroupConfig(std::string _description);// NOLINT(google-explicit-constructor)
    GroupConfig(const char* _description);// NOLINT(google-explicit-constructor)

    GroupConfig& setDescription(std::string _description);
};

struct TestCaseDefiner {
    TestCaseDefiner(void (*testCase)(), const char* name);
};

void test(TestConfig config, const std::function<void()>& func);

void group(GroupConfig config, const std::function<void()>& func);

void setUp(const std::function<void()>& func);

void tearDown(const std::function<void()>& func);

void fail(const std::string& message = std::string());

void expect(bool expr, const std::string& message = std::string());

}

#define KK_TEST_CASE_DEF_AUX_1_(x, y) x##_##y
#define KK_TEST_CASE_DEF_AUX_2_(x, y) KK_TEST_CASE_DEF_AUX_1_(x, y)
#define kkTestCase(NAME)                                                       \
            KK_TEST_CASE_DEF_AUX_2_(kkTestCaseInstance, NAME)();               \
            kktest::TestCaseDefiner                                            \
                    KK_TEST_CASE_DEF_AUX_2_(kkTestCaseDefiner, NAME)(          \
                            KK_TEST_CASE_DEF_AUX_2_(kkTestCaseInstance, NAME), \
                            #NAME);                                            \
            void KK_TEST_CASE_DEF_AUX_2_(kkTestCaseInstance, NAME)()

#endif
