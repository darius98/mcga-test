#ifndef COMMON_LOGGING_INCLUDE_LOGGING_PUBLIC_TEST_LOGGER_H_
#define COMMON_LOGGING_INCLUDE_LOGGING_PUBLIC_TEST_LOGGER_H_

#include <ostream>
#include <set>
#include <string>


namespace logging {

class TestLogger {
public:
    explicit TestLogger(std::ostream& _stream, bool _maintainTestIndexOrder=true);

    void logTest(int testIndex,
                 const std::string& groupDescription,
                 const std::string& description,
                 bool isOptional,
                 bool isPassed,
                 const std::string& failureMessage);

    void logFinalInformation(bool logNumTests=false);

private:
    std::string getTestMessage(const std::string& groupDescription,
                               const std::string& description,
                               bool isOptional,
                               bool isPassed,
                               std::string failureMessage);

    bool isInTerminal() const;

    void modifyOutput(const int& code, std::ostream& streamToChange);

    std::ostream& stream;

    bool maintainTestIndexOrder;
    int testCasesReceived = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int testsLogged = 0;
    std::set<std::pair<int, std::string>> testsQueue;
};

}

#endif
