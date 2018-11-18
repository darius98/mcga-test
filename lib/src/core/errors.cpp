#include "errors.hpp"

using namespace std;


namespace kktest {

ExpectationFailed::ExpectationFailed(const string& str): runtime_error(str) {}

}
