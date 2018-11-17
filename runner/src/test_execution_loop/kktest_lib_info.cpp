#include "kktest_lib_info.hpp"

using namespace std;

namespace runner {

string KKTestCaseInfo::getRecursiveGroupDescription(int groupId) const {
    auto groupInfoIterator = groups.find(groupId);
    if (groupInfoIterator == groups.end()) {
        return "";
    }
    GroupInfo groupInfo = groupInfoIterator->second;
    return getRecursiveGroupDescription(groupInfo.parentGroupIndex) + groupInfo.description + "::";
}

}
