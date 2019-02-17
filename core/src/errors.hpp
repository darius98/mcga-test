#ifndef KKTEST_CORE_SRC_ERRORS_HPP_
#define KKTEST_CORE_SRC_ERRORS_HPP_

#include <stdexcept>

namespace kktest {

class ConfigurationError: public std::runtime_error {
 public:
    explicit ConfigurationError(const std::string& str);
};

class Bug: public std::runtime_error {
 public:
    explicit Bug(const std::string& str);
};

class ExpectationFailed: public std::runtime_error {
 public:
    explicit ExpectationFailed(const std::string& str);
};

}

#endif
