#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include <memory>
#include <string>

#include "common/interproc/src/message.hpp"
#include "core/include/kktest_impl/config.hpp"
#include "core/src/info.hpp"

namespace kktest {

class Group;

class Test {
 public:
    Test(const TestConfig& _config, Group* _parentGroup, int _index);

    const TestConfig& getConfig() const;

    Group* getGroup() const;

    int getIndex() const;

    TestInfo toTestInfo() const;

 private:
    TestConfig config;
    Group* parentGroup;
    int index;

friend class Driver;
};

}

#endif
