#pragma once

#include <ostream>

namespace mcga::test {

class DeathStatus {
  public:
    constexpr DeathStatus() = default;
    constexpr DeathStatus(int exitCode, int signal)
            : exitCode(exitCode), signal(signal) {
    }

    [[nodiscard]] constexpr bool exitedOrKilled() const {
        return exitCode >= 0 || killedBySignal();
    }

    [[nodiscard]] constexpr bool exited() const {
        return exitCode >= 0 && !killedBySignal();
    }

    [[nodiscard]] constexpr bool killedBySignal() const {
        return signal > 0;
    }

    [[nodiscard]] constexpr int getExitCode() const {
        return exitCode;
    }

    [[nodiscard]] constexpr int getSignal() const {
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

}  // namespace mcga::test
