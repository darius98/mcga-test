#ifndef KKTEST_INCLUDE_KKTEST_IMPL_INFO_H_
#define KKTEST_INCLUDE_KKTEST_IMPL_INFO_H_

#include <string>

namespace kktest {

struct TestInfo {
    int groupIndex;
    int index;
    int line;
    std::string file;
    bool optional;
    std::string description;
    bool passed;
    std::string failureMessage;
};

struct GroupInfo {
    int parentGroupIndex;
    int index;
    int line;
    std::string file;
    std::string description;
};

}

#endif
