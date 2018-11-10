#ifndef KKTEST_CORE_TEST_H_
#define KKTEST_CORE_TEST_H_

#include <string>

#include <core/test_config.hpp>
#include <utils/message.hpp>
#include "expectation_failed.hpp"


namespace kktest {

class Group;

class Test: public MessageSerializable {
public:
    Test(const TestConfig& _config,
         std::string _file,
         int _line,
         Group* _parentGroup,
         int _index);

    ~Test() override;

    const TestConfig& getConfig() const;

    int getIndex() const;

    std::string getFilename() const;

    int getLine() const;

    void setExecuted(double _executionTimeTicks);

    bool isExecuted() const;

    bool isFailed() const;

    bool isPassed() const;

    double getExecutionTimeTicks() const;

    std::string getFailureMessage() const;

    void setFailure(const std::string& message);

    void setFailure(const ExpectationFailed& f);

    std::string getDescriptionPrefix() const;

    void writeBytes(BytesConsumer& consumer) const override;

    Group* getParentGroup() const;
private:

    TestConfig config;

    std::string file;
    int line;

    Group* parentGroup;
    int index;

    bool executed = false;
    ExpectationFailed* failure = nullptr;
    double executionTimeTicks = -1.0;
};

}

#endif
