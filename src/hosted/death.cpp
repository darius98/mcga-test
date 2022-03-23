#include "mcga/test_ext/death/check_death.hpp"

#include <thread>

#include "mcga/proc.hpp"

#include "core/driver.hpp"
#include "core/export.hpp"
#include "core/time_tick.hpp"

namespace mcga::test {
inline namespace MCGA_TEST_INTERNAL_ABI_NS {

MCGA_TEST_EXPORT DeathStatus checkDeath(Executable func,
                                        double timeTicksLimit) {
    if (Driver::Instance()->getExecutorType() == Executor::Type::SMOOTH) {
        fail("Death extension matchers & the checkDeath function do not work"
             " when using a smooth executor.");
        return {-1, -1};
    }

    proc::WorkerSubprocess proc(
      TimeTicksToNanoseconds(timeTicksLimit),
      [func = std::move(func)](std::unique_ptr<proc::PipeWriter> writer) {
          func();
          writer->sendMessage(1);
      });

    while (!proc.isFinished()) {
        std::this_thread::sleep_for(std::chrono::milliseconds{5});
    }
    if (proc.getNextMessage(1).isInvalid()) {
        return {proc.getReturnCode(), proc.getSignal()};
    }
    return {-1, -1};
}

}  // namespace MCGA_TEST_INTERNAL_ABI_NS
}  // namespace mcga::test
