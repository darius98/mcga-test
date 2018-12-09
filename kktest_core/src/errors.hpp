#ifndef KKTEST_CORE_SRC_ERRORS_HPP_
#define KKTEST_CORE_SRC_ERRORS_HPP_

#include <stdexcept>

#include <kktest_common/strutil.hpp>

namespace kktest {

class ConfigurationError: public std::runtime_error {
 public:
    explicit ConfigurationError(const String& str);
};

class KKTestLibraryImplementationError: public std::runtime_error {
 public:
    explicit KKTestLibraryImplementationError(const String& str);
};

class ExpectationFailed: public std::runtime_error {
 public:
    explicit ExpectationFailed(const String& str);
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_ERRORS_HPP_
