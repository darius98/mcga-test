#ifndef KKTEST_CORE_SRC_ERRORS_HPP_
#define KKTEST_CORE_SRC_ERRORS_HPP_

#include <stdexcept>

namespace kktest {

struct ConfigurationError : std::runtime_error {
    explicit ConfigurationError(const std::string& str);
};

struct Bug : std::runtime_error {
    explicit Bug(const std::string& str);
};

struct ExpectationFailed : std::runtime_error {
    explicit ExpectationFailed(const std::string& str);
};

}

#endif
