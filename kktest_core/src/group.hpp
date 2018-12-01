#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <string>

#include <kktest_impl/types.hpp>
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

    String getRenderedFailureMessageOnExceptionInSetUp(const String& what) const;

    String getRenderedFailureMessageOnNonExceptionInSetUp() const;

    String getRenderedFailureMessageOnExceptionInTearDown(const String& what) const;

    String getRenderedFailureMessageOnNonExceptionInTearDown() const;

private:
    GroupConfig config;

    Group* parentGroup;
    int index;

    CopyableExecutable setUpFunc;
    CopyableExecutable tearDownFunc;
};

}

#endif
