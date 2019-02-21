#ifndef KKTEST_CORE_SRC_GROUP_HPP_
#define KKTEST_CORE_SRC_GROUP_HPP_

#include <memory>

#include "core/include/kktest.hpp"

namespace kktest {

class Group : private GroupConfig {
 public:
    typedef std::shared_ptr<Group> Ptr;

    Group(GroupConfig&& config, Ptr parentGroup, int index);

    std::string getDescription() const;

    int getIndex() const;

    Ptr getParentGroup() const;

    void addSetUp(Executable func);

    void setUp() const;

    void addTearDown(Executable func);

    void tearDown() const;

    void addStartedTest();

    void setStartedAllTests();

    void addFinishedTest();

    bool finishedAllTests() const;

 private:
    Ptr parentGroup;
    int index;

    int testsStarted = 0;
    bool allTestsStarted = false;
    int testsFinished = 0;

    std::function<void()> setUpFunc;
    std::function<void()> tearDownFunc;
};

typedef std::shared_ptr<Group> GroupPtr;

}

#endif
