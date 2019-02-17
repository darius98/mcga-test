#ifndef KKTEST_CORE_SRC_INFO_HPP_
#define KKTEST_CORE_SRC_INFO_HPP_

#include <string>

namespace kktest {

struct TestInfo {
    int groupIndex = -1;
    int index = -1;
    bool optional = false;
    std::string description = "-";
    bool passed = true;
    std::string failureMessage = "";

    TestInfo();
    TestInfo(int _groupIndex,
             int _index,
             bool _optional,
             std::string _description,
             bool _passed,
             std::string _failureMessage);

    bool operator<(const TestInfo& other) const;
};

struct GroupInfo {
    int parentGroupIndex = -1;
    int index = -1;
    std::string description = "";

    GroupInfo();
    GroupInfo(int _parentGroupIndex, int _index, std::string _description);

    bool operator<(const GroupInfo& other) const;
};

}

#endif
