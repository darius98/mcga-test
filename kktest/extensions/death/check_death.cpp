#include "kktest/extensions/death/include/kktest_ext/death_impl/check_death.hpp"

#include <thread>

#include "common/interproc/worker_subprocess.hpp"
#include "kktest/core/driver.hpp"
#include "kktest/core/export.hpp"
#include "kktest/core/time_tick.hpp"

using namespace mcga::proc;
using namespace std;

namespace mcga::test::death {

KKTEST_EXPORT DeathStatus checkDeath(const Executable& func,
                                     double timeTicksLimit) {
    if (Driver::Instance()->getExecutorType() == Executor::SMOOTH) {
        fail("Death extension matchers & the checkDeath function do not work"
             " when using a smooth executor.");
        return DeathStatus(-1, -1);
    }

    WorkerSubprocess proc(TimeTicksToNanoseconds(timeTicksLimit),
                          [func](PipeWriter* writer) {
                              func();
                              writer->sendMessage(1);
                          });

    while (!proc.isFinished()) {
        std::this_thread::sleep_for(5ms);
    }
    if (proc.getNextMessage(32).isInvalid()) {
        // The exit was ok.
        return DeathStatus(proc.getReturnCode(), proc.getSignal());
    }
    return DeathStatus(-1, -1);
}

}  // namespace mcga::test::death
