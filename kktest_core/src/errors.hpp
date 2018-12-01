#ifndef KKTEST_CORE_ERRORS_H_
#define KKTEST_CORE_ERRORS_H_

#include <stdexcept>

#include <kktest_impl/types.hpp>

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

}

#endif
