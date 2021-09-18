#include "warning.hpp"

namespace mcga::test {

Warning::Warning(std::string message, int groupId)
        : message(std::move(message)), groupId(groupId) {
}

Warning::Warning(std::string message, int groupId, int testId)
        : message(std::move(message)), groupId(groupId), testId(testId) {
}

}  // namespace mcga::test
