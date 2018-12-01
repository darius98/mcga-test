#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_INFO_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_INFO_H_

#include <kktest_impl/types.hpp>

namespace kktest {

struct TestInfo {
    int groupIndex;
    int index;
    bool optional;
    String description;
    bool passed;
    String failureMessage;
};

struct GroupInfo {
    int parentGroupIndex;
    int index;
    String description;
};

}

#endif
