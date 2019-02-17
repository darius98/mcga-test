#include "extensions/death/include/kktest_ext/death_impl/death_status.hpp"

using namespace std;

namespace kktest {
namespace death {

DeathStatus::DeathStatus() = default;

bool DeathStatus::exitedOrKilled() const {
    return exitCode >= 0 || killedBySignal();
}

bool DeathStatus::exited() const {
    return exitCode >= 0 && !killedBySignal();
}

bool DeathStatus::killedBySignal() const {
    return signal > 0;
}

int DeathStatus::getExitCode() const {
    return exitCode;
}

int DeathStatus::getSignal() const {
    return signal;
}

string DeathStatus::getOutput() const {
    return output;
}

DeathStatus::DeathStatus(int _exitCode, int _signal, string _output):
        exitCode(_exitCode), signal(_signal), output(move(_output)) {}

ostream& operator<<(ostream& out, const DeathStatus& status) {
    out << "(exit_code="
        << status.exitCode
        << ", signal="
        << status.signal
        << ")";
    return out;
}

}
}
