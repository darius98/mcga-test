#include "common/interproc/src/subprocess.hpp"

namespace kktest {
namespace interproc {

Subprocess::FinishStatus Subprocess::getFinishStatus() {
    if (isSignaled()) {
        return SIGNALED;
    }
    if (!isExited()) {
        return UNKNOWN;
    }
    return getReturnCode() == 0 ? ZERO_EXIT : NON_ZERO_EXIT;
}

}
}
