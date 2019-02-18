#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include <memory>
#include <string>

#include "core/src/group.hpp"

namespace kktest {

class Test {
 public:
    Test(TestConfig&& _config, Group* _group, int _index);
    Test(Test&& other) noexcept;

    // TODO(darius98): Explicitly delete this constructor!
    //  A test should never have to be copied, only passed around.
    //  ...
    //  Currently, it is needed for the BoxedExecutor, since we need a copy
    //  to send to the test process and one to keep.
    Test(const Test& other);

    Test& operator=(Test&& other) noexcept;

    std::string getDescription() const;

    bool isOptional() const;

    double getTimeTicksLimit() const;

    Group* getGroup() const;

    int getIndex() const;

 private:
    TestConfig config;
    Group* group;
    int index;
};

}

#endif
