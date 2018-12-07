#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_INFO_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_INFO_HPP_

#include <kktest_impl/types.hpp>

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

}  // namespace kktest

#endif  // KKTEST_CORE_INCLUDE_KKTEST_IMPL_INFO_HPP_
