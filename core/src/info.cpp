#include "core/src/info.hpp"

using namespace std;

namespace kktest {

TestInfo::TestInfo(int _groupIndex,
                   int _index,
                   bool _optional,
                   String _description,
                   bool _passed,
                   String _failureMessage):
        groupIndex(_groupIndex),
        index(_index),
        optional(_optional),
        description(move(_description)),
        passed(_passed),
        failureMessage(move(_failureMessage)) {}

bool TestInfo::operator<(const TestInfo& other) const {
    return index < other.index;
}

GroupInfo::GroupInfo() = default;

GroupInfo::GroupInfo(int _parentGroupIndex, int _index, String _description):
        parentGroupIndex(_parentGroupIndex),
        index(_index),
        description(move(_description)) {}

bool GroupInfo::operator<(const GroupInfo& other) const {
    return index < other.index;
}

}
