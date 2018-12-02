#include <string>

#include "errors.hpp"

using std::runtime_error;

namespace kktest {

ConfigurationError::ConfigurationError(const String& str): runtime_error(std::string(str)) {}

KKTestLibraryImplementationError::KKTestLibraryImplementationError(const String& str):
        runtime_error(std::string(str)) {}

ExpectationFailed::ExpectationFailed(const String& str): runtime_error(std::string(str)) {}

}
