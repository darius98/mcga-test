#include <kktest_ext/death_impl/death_status.hpp>

using namespace std;

namespace kktest {
namespace death {

DeathStatus::DeathStatus() = default;

bool DeathStatus::exitedOrKilled() const {
    return exitCode >= 0;
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
    return output.str();
}

DeathStatus::DeathStatus(int _exitCode, int _signal, String _output):
        exitCode(_exitCode), signal(_signal), output(move(_output)) {}

}
}
