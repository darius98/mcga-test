#ifndef KKTEST_CORE_SRC_INFO_HPP_
#define KKTEST_CORE_SRC_INFO_HPP_

#include "common/string/src/string.hpp"

namespace kktest {

struct TestInfo {
    int groupIndex = -1;
    int index = -1;
    bool optional = false;
    String description = "-";
    bool passed = true;
    String failureMessage = "";

    TestInfo(int _groupIndex,
             int _index,
             bool _optional,
             String _description,
             bool _passed,
             String _failureMessage);

    bool operator<(const TestInfo& other) const;
};

struct GroupInfo {
    int parentGroupIndex = -1;
    int index = -1;
    String description = "";

    GroupInfo();
    GroupInfo(int _parentGroupIndex, int _index, String _description);

    bool operator<(const GroupInfo& other) const;
};

}

#endif
