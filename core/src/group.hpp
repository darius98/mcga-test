#ifndef KKTEST_CORE_SRC_GROUP_HPP_
#define KKTEST_CORE_SRC_GROUP_HPP_

#include "core/include/kktest_impl/executable.hpp"
#include "core/src/test.hpp"

namespace kktest {

class Group {
 public:
    Group(const GroupConfig& _config, Group* _parentGroup, int _index);

    std::string getDescription() const;

    int getIndex() const;

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

typedef const Group*const GroupPtr;

}

#endif
