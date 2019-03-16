#include "kktest/extensions/death/include/kktest_ext/death_impl/check_death.hpp"

#include "common/interproc/src/worker_subprocess.hpp"
#include "kktest/core/src/export.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;

namespace kktest::death {

KKTEST_EXPORT DeathStatus checkDeath(const Executable& func,
                                     double timeTicksLimit) {
    WorkerSubprocess proc(
        Duration::FromMs(timeTicksLimit * getTimeTickLengthMs()),
        [func](PipeWriter* writer) {
            func();
            writer->sendMessage(1);
        });

    while (!proc.isFinished()) {
        sleepForDuration(5_ms);
    }
    if (proc.getNextMessage(32).isInvalid()) {
        // The exit was ok.
        return DeathStatus(proc.getReturnCode(), proc.getSignal());
    }
    return DeathStatus(-1, 0);
}

}
