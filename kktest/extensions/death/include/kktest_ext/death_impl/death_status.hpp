#pragma once

#include <ostream>
#include <string>

#include <kktest.hpp>

namespace kktest::death {

class KKTEST_EXPORT DeathStatus {
 public:
    DeathStatus();

    bool exitedOrKilled() const;

    bool exited() const;

    bool killedBySignal() const;

    int getExitCode() const;

    int getSignal() const;

    std::string getOutput() const;

 private:
    DeathStatus(int exitCode, int signal, std::string output);

    int exitCode;
    int signal;
    std::string output;

friend DeathStatus checkDeath(const Executable& func, double timeTicksLimit);
friend std::ostream& operator<<(std::ostream& out, const DeathStatus& status);
};

}
