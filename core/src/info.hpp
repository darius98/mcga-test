#ifndef KKTEST_CORE_SRC_INFO_HPP_
#define KKTEST_CORE_SRC_INFO_HPP_

#include "common/string/src/string.hpp"

namespace kktest {

struct TestInfo {
    int groupIndex;
    int index;
    bool optional;
    String description;
    bool passed;
    String failureMessage;

    inline bool operator<(const TestInfo& other) const {
        return index < other.index;
    }
};

struct GroupInfo {
    int parentGroupIndex;
    int index;
    String description;

    inline bool operator<(const GroupInfo& other) const {
        return index < other.index;
    }
};

}

#endif
