#ifndef RUNTIME_TESTING_TESTING_TESTING_H_
#define RUNTIME_TESTING_TESTING_TESTING_H_

#include <functional>
#include <string>


#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ?                       \
                      __builtin_strrchr(__FILE__, '/') + 1 :                   \
                      __FILE__)


#define test(desc, func) \
        runtime_testing::_test(desc, func, __FILENAME__, __LINE__)

void group(
        const std::string& description,
        const std::function<void()>& groupFunc);

#define setUp(func) \
        runtime_testing::_setUp(func, __FILENAME__, __LINE__)
#define tearDown(func) \
        runtime_testing::_tearDown(func, __FILENAME__, __LINE__)
#define getTestReport() \
        runtime_testing::_getTestReport(__FILENAME__)


namespace runtime_testing {

bool isDuringTest();

int numFailedTests();

void _test(
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
        const char* fileName,
        const int& lineNumber);

int _getTestReport(const char* fileName);

}

#endif
