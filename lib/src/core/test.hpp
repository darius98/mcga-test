#ifndef KKTEST_CORE_TEST_H_
#define KKTEST_CORE_TEST_H_

#include <string>

#include <utils/message.hpp>
#include "expectation_failed.hpp"


namespace kktest {

class Group;

class Test: public MessageSerializable {
public:
    Test(std::string _description,
         std::string _file,
         int _line,
         Group* _parentGroup,
         int _index);

    ~Test() override;

    int getIndex() const;

    std::string getFilename() const;

    int getLine() const;

    void setExecuted();

    bool isExecuted() const;

    bool isFailed() const;

    bool isPassed() const;

    std::string getFailureMessage() const;

    void setFailure(const std::string& message);

    void setFailure(const ExpectationFailed& f);

    std::string getDescriptionPrefix() const;

    std::string getDescription() const;

    void writeBytes(BytesConsumer& consumer) const override;

    Group* getParentGroup() const;
private:

    std::string description;

    std::string file;
    int line;

    Group* parentGroup;
    int index;

    bool executed = false;
    ExpectationFailed* failure = nullptr;
};

}

#endif
