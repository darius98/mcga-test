#pragma once

#include <ostream>
#include <string>

namespace kktest::death {

class DeathStatus {
 public:
    DeathStatus() = default;
    DeathStatus(int exitCode, int signal): exitCode(exitCode), signal(signal) {}

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

 private:
    int exitCode = -1;
    int signal = -1;

    friend std::ostream& operator<<(std::ostream& out, const DeathStatus& ds) {
        if (ds.exitCode != -1) {
            out << "<EXIT " << ds.exitCode << ">";
        } else if (ds.signal != -1) {
            out << "<KILLED BY SIGNAL " << ds.signal << ">";
        } else {
            out << "<UNKNOWN EXIT>";
        }
        return out;
    }
};

}
