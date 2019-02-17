#include "core/src/errors.hpp"

#include <string>

using namespace std;

namespace kktest {

ConfigurationError::ConfigurationError(const string& str): runtime_error(str) {}

Bug::Bug(const string& str): runtime_error(str) {}

ExpectationFailed::ExpectationFailed(const string& str): runtime_error(str) {}

}
