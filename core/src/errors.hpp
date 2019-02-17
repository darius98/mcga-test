#ifndef KKTEST_CORE_SRC_ERRORS_HPP_
#define KKTEST_CORE_SRC_ERRORS_HPP_

#include <stdexcept>

#include "common/string/src/string.hpp"

namespace kktest {

class ConfigurationError: public std::runtime_error {
 public:
    explicit ConfigurationError(const String& str);
};

class Bug: public std::runtime_error {
 public:
    explicit Bug(const String& str);
};

class ExpectationFailed: public std::runtime_error {
 public:
    explicit ExpectationFailed(const String& str);
};

}

#endif
