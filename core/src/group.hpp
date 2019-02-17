#ifndef KKTEST_CORE_SRC_GROUP_HPP_
#define KKTEST_CORE_SRC_GROUP_HPP_

#include "common/string/src/string.hpp"
#include "core/include/kktest_impl/executable.hpp"
#include "core/src/info.hpp"
#include "core/src/test.hpp"

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
