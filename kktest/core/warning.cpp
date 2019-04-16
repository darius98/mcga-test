#include "kktest/core/warning.hpp"

using namespace std;

namespace kktest {

Warning::Warning(string message, int groupId)
        : message(move(message)), groupId(groupId) {
}

Warning::Warning(string message, int groupId, int testId)
        : message(move(message)), groupId(groupId), testId(testId) {
}

}  // namespace kktest
