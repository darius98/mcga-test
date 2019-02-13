#include <kktest_common/interproc.hpp>
#include <kktest_impl/definers.hpp>
#include <kktest_ext/death_impl/check_death.hpp>

using namespace kktest::interproc;
using namespace std;

namespace kktest {
namespace death {

DeathStatus checkDeath(const function<void()>& func) {
    auto proc = WorkerSubprocess::open([func](PipeWriter* writer) {
        func();
        writer->sendMessage(1);
    });

    proc.wait();
    if (proc.getNextMessage(32).isInvalid()) {
        // The exit was ok.
        return DeathStatus(proc.getReturnCode(),
                           proc.getSignal(),
                           proc.getOutput());
    }
    return DeathStatus(-1, 0, "");
}

}
}
