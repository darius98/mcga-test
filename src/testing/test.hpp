#ifndef RUNTIME_TESTING_TESTING_TEST_H_
#define RUNTIME_TESTING_TESTING_TEST_H_

#include <functional>
#include <string>


#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ?                       \
                      __builtin_strrchr(__FILE__, '/') + 1 :                   \
                      __FILE__)


namespace runtime_testing {

bool isDuringTest();

void _test(
        const std::string& description,
        std::function<void()> testFunc,
        const char* filename,
        const int& lineNumber);

void _group(
        const std::string& description,
        const std::function<void()>& groupFunc,
        const char* fileName,
        const int& lineNumber);

void _setUp(
        std::function<void()> setUpFunc,
        const char* fileName,
        const int& lineNumber);

void _tearDown(
        std::function<void()> tearDownFunc,
        const char* filename,
        const int& lineNumber);

int getTestReport();

int numFailedTests();

}

#define test(desc, func) \
        runtime_testing::_test(desc, func, __FILENAME__, __LINE__)
#define group(desc, func) \
        runtime_testing::_group(desc, func, __FILENAME__, __LINE__)
#define setUp(func) \
        runtime_testing::_setUp(func, __FILENAME__, __LINE__)
#define tearDown(func) \
        runtime_testing::_tearDown(func, __FILENAME__, __LINE__)

#endif
