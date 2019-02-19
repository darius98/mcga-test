#ifndef KKTEST_CORE_INCLUDE_KKTEST_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_HPP_

#include <functional>
#include <string>

#define KKTEST_VERSION "1.0.0"

#if defined _WIN32 || defined __CYGWIN__
    #define KKTEST_EXPORT __declspec(dllexport)
#else
    #if __GNUC__ >= 4
        #define KKTEST_EXPORT __attribute__((visibility("default")))
    #else
        #define KKTEST_EXPORT
    #endif
#endif

namespace kktest {

struct KKTEST_EXPORT TestConfig {
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;

    TestConfig() = default;
    TestConfig(std::string _description); // NOLINT(google-explicit-constructor)
    TestConfig(const char* _description); // NOLINT(google-explicit-constructor)

    TestConfig& setDescription(std::string _description);
    TestConfig& setOptional(bool _optional);
    TestConfig& setTimeTicksLimit(double _timeTicksLimit);
};

struct KKTEST_EXPORT GroupConfig {
    std::string description = "-";

    GroupConfig() = default;
    GroupConfig(std::string _description);// NOLINT(google-explicit-constructor)
    GroupConfig(const char* _description);// NOLINT(google-explicit-constructor)

    GroupConfig& setDescription(std::string _description);
};

struct KKTEST_EXPORT TestCaseDefiner {
    TestCaseDefiner(void (*testCase)(), const char* name);
};

KKTEST_EXPORT void test(TestConfig config, const std::function<void()>& func);

KKTEST_EXPORT void group(GroupConfig config, const std::function<void()>& func);

KKTEST_EXPORT void setUp(const std::function<void()>& func);

KKTEST_EXPORT void tearDown(const std::function<void()>& func);

KKTEST_EXPORT void fail(const std::string& message = std::string());

KKTEST_EXPORT void expect(bool expr, const std::string& message=std::string());

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
