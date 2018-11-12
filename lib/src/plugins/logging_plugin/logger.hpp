#ifndef KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGER_H_
#define KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGER_H_

#include <ostream>
#include <set>

#include <core/test.hpp>


namespace kktest {

class Logger {
public:
    explicit Logger(std::ostream& _stream);

    void enqueueTestForLogging(Test* test);

    void logFinalInformation(int passedTests, int failedTests, int failedOptionalTests);

private:
    std::string getTestMessage(Test* test);

    bool isInTerminal() const;

    void modifyOutput(const int& code, std::ostream& streamToChange);

    std::ostream& stream;

    int testsLogged = 0;
    std::set<std::pair<int, std::string>> testsQueue;
};

}

#endif
