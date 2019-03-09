#pragma once

#include <ostream>
#include <string>

namespace kktest::death {

class DeathStatus {
 public:
    DeathStatus() = default;
    DeathStatus(int exitCode, int signal, std::string output):
            exitCode(exitCode), signal(signal), output(std::move(output)) {}

    bool exitedOrKilled() const {
        return exitCode >= 0 || killedBySignal();
    }

    bool exited() const {
        return exitCode >= 0 && !killedBySignal();
    }

    bool killedBySignal() const {
        return signal > 0;
    }

    int getExitCode() const {
        return exitCode;
    }

    int getSignal() const {
        return signal;
    }

    std::string getOutput() const {
        return output;
    }

 private:

    int exitCode = -1;
    int signal = -1;
    std::string output = "";

    friend std::ostream& operator<<(std::ostream& out,
                                    const DeathStatus& status) {
        out << "(code="
            << status.exitCode
            << ", signal="
            << status.signal
            << ")";
        return out;
    }
};

}
