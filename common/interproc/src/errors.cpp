#include "common/interproc/src/errors.hpp"

using namespace std;

namespace kktest {
namespace interproc {

InterprocError::InterprocError(const string& what): runtime_error(what) {}

}
}
