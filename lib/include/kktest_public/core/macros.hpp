#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MACROS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MACROS_H_

#include "./definers.hpp"
#include "./filename.hpp"

#define test kktest::TestDefiner(__FILENAME__, __LINE__)

#define group kktest::GroupDefiner(__FILENAME__, __LINE__)

#define setUp kktest::SetUpDefiner(__FILENAME__, __LINE__)

#define tearDown kktest::TearDownDefiner(__FILENAME__, __LINE__)

#define expect(...) kktest::ExpectDefiner(__FILENAME__, __LINE__)                                  \
                                         (__VA_ARGS__, #__VA_ARGS__ " is false")

#define failTest(...) kktest::ExpectDefiner(__FILENAME__, __LINE__)(false, __VA_ARGS__)

#endif
