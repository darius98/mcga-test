#include "expectation_failed.hpp"

using namespace std;


namespace kktest {

ExpectationFailed::ExpectationFailed(const string& str): runtime_error(str) {}

}
