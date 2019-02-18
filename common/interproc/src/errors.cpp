#include "common/interproc/src/errors.hpp"

using namespace std;

namespace kktest {
namespace interproc {

InterprocError::InterprocError(const string& _what): runtime_error(_what) {}

}
}
