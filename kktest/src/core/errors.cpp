#include <core/errors.hpp>

using namespace std;

namespace kktest {

ConfigurationError::ConfigurationError(const string& str): runtime_error(str) {}

KKTestLibraryImplementationError::KKTestLibraryImplementationError(const string& str):
        runtime_error(str) {}

ExpectationFailed::ExpectationFailed(const string& str): runtime_error(str) {}

}
