#pragma once

#include <stdexcept>

namespace kktest {

struct ConfigurationError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct ExpectationFailed : std::runtime_error {
    using std::runtime_error::runtime_error;
};

}
