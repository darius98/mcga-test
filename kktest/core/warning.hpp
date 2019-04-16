#pragma once

#include <string>

namespace mcga::test {

struct Warning {
    std::string message;
    int groupId;
    int testId = 0;

    Warning(std::string message, int groupId);
    Warning(std::string message, int groupId, int testId);
};

}  // namespace mcga::test
