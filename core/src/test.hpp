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
    Test& operator=(Test&& other) noexcept;

    Test(const Test& other) = delete;
    Test& operator=(const Test& other) = delete;

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
