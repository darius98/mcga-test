#include "warning.hpp"

using std::string;

namespace mcga::test {

Warning::Warning(string message, int groupId)
        : message(move(message)), groupId(groupId) {
}

Warning::Warning(string message, int groupId, int testId)
        : message(move(message)), groupId(groupId), testId(testId) {
}

}  // namespace mcga::test
