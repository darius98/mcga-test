#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_INFO_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_INFO_H_

#include <string>

namespace kktest {

struct TestInfo {
    int groupIndex;
    int index;
    bool optional;
    std::string description;
    bool passed;
    std::string failureMessage;
};

struct GroupInfo {
    int parentGroupIndex;
    int index;
    std::string description;
};

}

#endif
