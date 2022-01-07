#pragma once

#include "core/export.hpp"

#define MCGA_CAT(a, b) MCGA_INTERNAL_CAT(a, b)
#define MCGA_INTERNAL_CAT(a, b) a##b

#define MCGA_COMMA(...) ,

#define MCGA_COMMA_IF(i) MCGA_CAT(MCGA_INTERNAL_COMMA_IF_, i)
#define MCGA_INTERNAL_COMMA_IF_0
#define MCGA_INTERNAL_COMMA_IF_1 ,
#define MCGA_INTERNAL_COMMA_IF_2 ,
#define MCGA_INTERNAL_COMMA_IF_3 ,
#define MCGA_INTERNAL_COMMA_IF_4 ,
#define MCGA_INTERNAL_COMMA_IF_5 ,
#define MCGA_INTERNAL_COMMA_IF_6 ,
#define MCGA_INTERNAL_COMMA_IF_7 ,
#define MCGA_INTERNAL_COMMA_IF_8 ,
#define MCGA_INTERNAL_COMMA_IF_9 ,
#define MCGA_INTERNAL_COMMA_IF_10 ,
#define MCGA_INTERNAL_COMMA_IF_11 ,
#define MCGA_INTERNAL_COMMA_IF_12 ,
#define MCGA_INTERNAL_COMMA_IF_13 ,
#define MCGA_INTERNAL_COMMA_IF_14 ,
#define MCGA_INTERNAL_COMMA_IF_15 ,

#define MCGA_HEAD(...) MCGA_HEAD_1((__VA_ARGS__, UNUSED_ARG))
#define MCGA_HEAD_1(Args) MCGA_HEAD_2 Args
#define MCGA_HEAD_2(a, ...) a

#define MCGA_TAIL(...) MCGA_TAIL_1((__VA_ARGS__))
#define MCGA_TAIL_1(Args) MCGA_TAIL_2 Args
#define MCGA_TAIL_2(a, ...) __VA_ARGS__

#define MCGA_IF(Cond, Then, Else) MCGA_CAT(MCGA_IF_, Cond)(Then, Else)
#define MCGA_IF_0(Then, Else) Else
#define MCGA_IF_1(Then, Else) Then

#define MCGA_REPEAT(Macro, Data, N)                                            \
    MCGA_CAT(MCGA_REPEAT_, N)(0, Macro, Data, (, , , , , , , , , , , , , , , ))
#define MCGA_REPEAT_0(i, Macro, Data, Tuple)
#define MCGA_REPEAT_1(i, Macro, Data, Tuple) Macro(i, Data, MCGA_HEAD Tuple)
#define MCGA_REPEAT_2(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_1(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_3(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_2(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_4(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_3(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_5(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_4(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_6(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_5(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_7(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_6(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_8(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_7(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_9(i, Macro, Data, Tuple)                                   \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_8(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_10(i, Macro, Data, Tuple)                                  \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_9(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_11(i, Macro, Data, Tuple)                                  \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_10(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_12(i, Macro, Data, Tuple)                                  \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_11(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_13(i, Macro, Data, Tuple)                                  \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_12(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_14(i, Macro, Data, Tuple)                                  \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_13(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))
#define MCGA_REPEAT_15(i, Macro, Data, Tuple)                                  \
    Macro(i, Data, MCGA_HEAD Tuple)                                            \
      MCGA_REPEAT_14(MCGA_INC(i), Macro, Data, (MCGA_TAIL Tuple))

#define MCGA_INC(i) MCGA_CAT(MCGA_INC_, i)
#define MCGA_INC_0 1
#define MCGA_INC_1 2
#define MCGA_INC_2 3
#define MCGA_INC_3 4
#define MCGA_INC_4 5
#define MCGA_INC_5 6
#define MCGA_INC_6 7
#define MCGA_INC_7 8
#define MCGA_INC_8 9
#define MCGA_INC_9 10
#define MCGA_INC_10 11
#define MCGA_INC_11 12
#define MCGA_INC_12 13
#define MCGA_INC_13 14
#define MCGA_INC_14 15
#define MCGA_INC_15 16

#define MCGA_DEC(i) MCGA_CAT(MCGA_DEC_, i)
#define MCGA_DEC_1 0
#define MCGA_DEC_2 1
#define MCGA_DEC_3 2
#define MCGA_DEC_4 3
#define MCGA_DEC_5 4
#define MCGA_DEC_6 5
#define MCGA_DEC_7 6
#define MCGA_DEC_8 7
#define MCGA_DEC_9 8
#define MCGA_DEC_10 9
#define MCGA_DEC_11 10
#define MCGA_DEC_12 11
#define MCGA_DEC_13 12
#define MCGA_DEC_14 13
#define MCGA_DEC_15 14
#define MCGA_DEC_16 15

#define INTERNAL_PARAM(i, mock, _) MCGA_COMMA_IF(i) mock::arg_t<i> arg##i
#define INTERNAL_PARAMS(mock, NArgs) MCGA_REPEAT(INTERNAL_PARAM, mock, NArgs)

#define INTERNAL_FORWARD_ARG(i, mock, _)                                       \
    MCGA_COMMA_IF(i)::std::forward<mock::arg_t<(i)>>(arg##i)
#define INTERNAL_FORWARD_ARGS(mock, NArgs)                                     \
    MCGA_REPEAT(INTERNAL_FORWARD_ARG, mock, NArgs)

#define INTERNAL_DECLARE_FUNCTION(mock_group,                                  \
                                  Func,                                        \
                                  NArgs,                                       \
                                  AfterParams,                                 \
                                  BeforeInvoke,                                \
                                  AfterInvokeArgs,                             \
                                  AfterInvoke)                                 \
    MCGA_TEST_EXPORT extern "C" typename decltype((mock_group).Func)::return_t \
      Func(INTERNAL_PARAMS(decltype((mock_group).Func), NArgs) AfterParams) {  \
        BeforeInvoke(mock_group)                                               \
          .Func.invoke(INTERNAL_FORWARD_ARGS(decltype((mock_group).Func),      \
                                             NArgs) AfterInvokeArgs);          \
        AfterInvoke                                                            \
    }

#define DECLARE_FUNCTION(mock_group, Func, NArgs)                              \
    INTERNAL_DECLARE_FUNCTION(mock_group, Func, NArgs, , return, , )

#define DECLARE_NORETURN_FUNCTION(mock_group, Func, NArgs)                     \
    INTERNAL_DECLARE_FUNCTION(                                                 \
      mock_group,                                                              \
      Func,                                                                    \
      NArgs,                                                                   \
      ,                                                                        \
      ,                                                                        \
      ,                                                                        \
      ::mcga::test::mock::internal::mcga_test_ext_after_noreturn_invoke();)

#define DECLARE_VA_FUNCTION(mock_group, Func, NArgs)                           \
    INTERNAL_DECLARE_FUNCTION(                                                 \
      mock_group, Func, NArgs, MCGA_COMMA(1)..., va_list va_args;              \
      va_start(va_args, MCGA_CAT(arg, MCGA_DEC(NArgs)));                       \
      const auto result =, MCGA_COMMA(1) va_args, va_end(va_args);             \
      return result;)
