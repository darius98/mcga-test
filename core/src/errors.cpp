#include "core/src/errors.hpp"

#include <string>

using namespace std;

namespace kktest {

ConfigurationError::ConfigurationError(const String& str):
        runtime_error(string(str)) {}

Bug::Bug(const String& str): runtime_error(string(str)) {}

ExpectationFailed::ExpectationFailed(const String& str):
        runtime_error(string(str)) {}

}
