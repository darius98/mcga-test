#include "mcga/test_ext/death/check_death.hpp"

#include <thread>

#include <mcga/proc.hpp>

#include "core/driver.hpp"
#include "core/export.hpp"
#include "core/time_tick.hpp"

using namespace mcga::proc;

namespace mcga::test::internal {

MCGA_TEST_EXPORT void check_death(Executable func,
                                  double timeTicksLimit,
                                  int* exitCode,
                                  int* exitSignal) {
    *exitCode = *exitSignal = -1;
    if (Driver::Instance()->getExecutorType() == Executor::SMOOTH) {
        // TODO: Don't fail() here, implement the skip() functionality instead!
        fail("Death extension matchers & the checkDeath function do not work"
             " when using a smooth executor.");
        return;
    }

    WorkerSubprocess proc(
      TimeTicksToNanoseconds(timeTicksLimit),
      [func = std::move(func)](std::unique_ptr<PipeWriter> writer) {
          func();
          writer->sendMessage(1);
      });

    while (!proc.isFinished()) {
        std::this_thread::sleep_for(std::chrono::milliseconds{5});
    }
    if (proc.getNextMessage(1).isInvalid()) {
        *exitCode = proc.getReturnCode();
        *exitSignal = proc.getSignal();
    }
}

}  // namespace mcga::test::internal
