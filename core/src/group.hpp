#ifndef KKTEST_CORE_GROUP_HPP_
#define KKTEST_CORE_GROUP_HPP_

#include <extension_api/info.hpp>
#include <kktest_common/string.hpp>
#include <kktest_impl/executable.hpp>
#include "test.hpp"

namespace kktest {

class Group {
 public:
    Group(const GroupConfig& _config, Group* _parentGroup, int _index);

    String getDescription() const;

    int getIndex() const;

    GroupInfo getGroupInfo() const;

    Group* getParentGroup() const;

    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

 private:
    GroupConfig config;

    Group* parentGroup;
    int index;

    CopyableExecutable setUpFunc;
    CopyableExecutable tearDownFunc;
};

}

#endif
