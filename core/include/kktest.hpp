#ifndef KKTEST_CORE_KKTEST_HPP_
#define KKTEST_CORE_KKTEST_HPP_

#include <kktest_impl/definers.hpp>
#include <kktest_impl/signature.hpp>

#define KKTESTCASE_DEF_AUX_1_(x, y) x##_##y
#define KKTESTCASE_DEF_AUX_2_(x, y) KKTESTCASE_DEF_AUX_1_(x, y)
#define kkTestCase(NAME)                                                       \
            KKTESTCASE_DEF_AUX_2_(kkTestCaseInstance, NAME)();                 \
            kktest::TestCaseDefiner                                            \
                    KKTESTCASE_DEF_AUX_2_(kkTestCaseDefiner, NAME)(            \
                            KKTESTCASE_DEF_AUX_2_(kkTestCaseInstance, NAME),   \
                            #NAME,                                             \
                            kktest::kkTestSignature);                          \
            void KKTESTCASE_DEF_AUX_2_(kkTestCaseInstance, NAME)()

#endif
