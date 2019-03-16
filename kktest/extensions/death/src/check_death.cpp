#include "kktest/extensions/death/include/kktest_ext/death_impl/check_death.hpp"

#include "common/interproc/src/worker_subprocess.hpp"
#include "kktest/core/src/export.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;
using namespace std::chrono;

namespace kktest::death {

KKTEST_EXPORT DeathStatus checkDeath(const Executable& func,
                                     double timeTicksLimit) {
    WorkerSubprocess proc(timeTicksToNanoseconds(timeTicksLimit),
                         [func](PipeWriter* writer) {
                             func();
                             writer->sendMessage(1);
                         });

    while (!proc.isFinished()) {
        sleep(5ms);
    }
    if (proc.getNextMessage(32).isInvalid()) {
        // The exit was ok.
        return DeathStatus(proc.getReturnCode(), proc.getSignal());
    }
    return DeathStatus(-1, 0);
}

}
