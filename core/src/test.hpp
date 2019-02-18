#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include <memory>
#include <string>

#include "common/interproc/src/message.hpp"
#include "core/include/kktest_impl/config.hpp"

namespace kktest {

class Group;

class Test {
 public:
    Test(const TestConfig& _config, Group* _parentGroup, int _index);

    const TestConfig& getConfig() const;

    std::string getDescription() const;

    bool isOptional() const;

    Group* getGroup() const;

    int getIndex() const;

 private:
    TestConfig config;
    Group* parentGroup;
    int index;

friend class Driver;
};

typedef const Test*const TestPtr;

}

#endif
