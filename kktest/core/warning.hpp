#pragma once

#include <string>

namespace kktest {

struct Warning {
    std::string message;
    int groupId;
    int testId = 0;

    Warning(std::string message, int groupId);
    Warning(std::string message, int groupId, int testId);
};

}
