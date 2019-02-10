#ifndef KKTEST_CORE_KKTEST_HPP_
#define KKTEST_CORE_KKTEST_HPP_

#include <kktest_impl/definers.hpp>
#include <kktest_impl/signature.hpp>

#define KK_TEST_CASE_DEF_AUX_1_(x, y) x##_##y
#define KK_TEST_CASE_DEF_AUX_2_(x, y) KK_TEST_CASE_DEF_AUX_1_(x, y)
#define kkTestCase(NAME)                                                       \
            KK_TEST_CASE_DEF_AUX_2_(kkTestCaseInstance, NAME)();               \
            kktest::TestCaseDefiner                                            \
                    KK_TEST_CASE_DEF_AUX_2_(kkTestCaseDefiner, NAME)(          \
                            KK_TEST_CASE_DEF_AUX_2_(kkTestCaseInstance, NAME), \
                            #NAME,                                             \
                            kktest::kkTestSignature);                          \
            void KK_TEST_CASE_DEF_AUX_2_(kkTestCaseInstance, NAME)()

#endif
