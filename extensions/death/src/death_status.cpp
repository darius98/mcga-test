#include "extensions/death/include/kktest_ext/death_impl/death_status.hpp"

#include "core/src/export.hpp"

using namespace std;

namespace kktest {
namespace death {

KKTEST_EXPORT DeathStatus::DeathStatus() = default;

KKTEST_EXPORT bool DeathStatus::exitedOrKilled() const {
    return exitCode >= 0 || killedBySignal();
}

KKTEST_EXPORT bool DeathStatus::exited() const {
    return exitCode >= 0 && !killedBySignal();
}

KKTEST_EXPORT bool DeathStatus::killedBySignal() const {
    return signal > 0;
}

KKTEST_EXPORT int DeathStatus::getExitCode() const {
    return exitCode;
}

KKTEST_EXPORT int DeathStatus::getSignal() const {
    return signal;
}

KKTEST_EXPORT string DeathStatus::getOutput() const {
    return output;
}

KKTEST_EXPORT DeathStatus::DeathStatus(int _exitCode,
                                       int _signal,
                                       string _output):
        exitCode(_exitCode), signal(_signal), output(move(_output)) {}

KKTEST_EXPORT ostream& operator<<(ostream& out, const DeathStatus& status) {
    out << "(exit_code="
        << status.exitCode
        << ", signal="
        << status.signal
        << ")";
    return out;
}

}
}
