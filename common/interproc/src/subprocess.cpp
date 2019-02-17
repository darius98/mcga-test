#include "common/interproc/src/subprocess.hpp"

namespace kktest {
namespace interproc {

Subprocess::FinishStatus Subprocess::getFinishStatus() {
    if (!isExited() && !isSignaled()) {
        return NO_EXIT;
    }
    if (isSignaled()) {
        return SIGNAL_EXIT;
    }
    if (getReturnCode() != 0) {
        return NON_ZERO_EXIT;
    }
    return ZERO_EXIT;
}

}
}
