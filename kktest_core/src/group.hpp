#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <string>

#include <kktest_impl/executable.hpp>
#include <kktest_impl/info.hpp>
#include "test.hpp"

namespace kktest {

class Group {
public:
    Group(const GroupConfig& _config, Group* _parentGroup, int _index);

    const GroupConfig& getConfig() const;

    int getIndex() const;

    GroupInfo getGroupInfo() const;

    Group* getParentGroup() const;

    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

    std::string getRenderedFailureMessageOnExceptionInSetUp(const std::string& what) const;

    std::string getRenderedFailureMessageOnNonExceptionInSetUp() const;

    std::string getRenderedFailureMessageOnExceptionInTearDown(const std::string& what) const;

    std::string getRenderedFailureMessageOnNonExceptionInTearDown() const;

private:
    GroupConfig config;

    Group* parentGroup;
    int index;

    CopyableExecutable setUpFunc;
    CopyableExecutable tearDownFunc;
};

}

#endif
