#ifndef KKTEST_CORE_TEST_H_
#define KKTEST_CORE_TEST_H_

#include <string>

#include <JSON>

#include "expectation_failed.hpp"


namespace kktest {

class Group;

class Test {
private:
    static int globalTestIndex;

public:
    Test(std::string _description,
         std::string _file,
         int _line,
         Group* _parentGroup);

    ~Test();

    int getIndex() const;

    void setExecuted();

    bool isExecuted() const;

    bool isFailed() const;

    bool isPassed() const;

    std::string getFailureMessage() const;

    void setFailure(const std::string& message);

    void setFailure(const ExpectationFailed& f);

    std::string getDescriptionPrefix() const;

    std::string getDescription() const;

    std::size_t numBytes() const;

    void writeBytes(uint8_t* dst) const;

    autojson::JSON toJSON() const;

    void loadFromJSON(const autojson::JSON& json);

    Group* getParentGroup() const;
private:
    Group* parentGroup;
    int index;

    std::string description;

    std::string file;
    int line;

    bool executed = false;
    ExpectationFailed* failure = nullptr;
};

}

#endif