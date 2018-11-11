#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MACROS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MACROS_H_

#include "./definers.hpp"
#include "./filename.hpp"

#define kkTest kktest::TestDefiner(__FILENAME__, __LINE__)

#define kkGroup kktest::GroupDefiner(__FILENAME__, __LINE__)

#define kkSetUp kktest::SetUpDefiner(__FILENAME__, __LINE__)

#define kkTearDown kktest::TearDownDefiner(__FILENAME__, __LINE__)

#define kkExpect(...) kktest::ExpectDefiner(__FILENAME__, __LINE__)                                \
                                         (__VA_ARGS__, #__VA_ARGS__ " is false")

#define kkFail(...) kktest::ExpectDefiner(__FILENAME__, __LINE__)(false, __VA_ARGS__)

#endif
