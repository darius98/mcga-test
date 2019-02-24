#include "kktest/extensions/death/include/kktest_ext/death_impl/death_status.hpp"

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

DeathStatus::DeathStatus(int exitCode, int signal, string output):
        exitCode(exitCode), signal(signal), output(move(output)) {}

ostream& operator<<(ostream& out, const DeathStatus& status) {
    out << "(code=" << status.exitCode << ", signal=" << status.signal << ")";
    return out;
}

}
}
