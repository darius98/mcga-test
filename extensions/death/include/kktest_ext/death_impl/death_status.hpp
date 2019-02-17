#ifndef KKTEST_EXTENSIONS_DEATH_INCLUDE_KKTEST_EXT_DEATH_IMPL_DEATH_STATUS_HPP_
#define KKTEST_EXTENSIONS_DEATH_INCLUDE_KKTEST_EXT_DEATH_IMPL_DEATH_STATUS_HPP_

#include <ostream>
#include <string>

#include <kktest_impl/executable.hpp>
#include <kktest_impl/export.hpp>

namespace kktest {
namespace death {

class KKTEST_API DeathStatus {
 public:
    DeathStatus();

    bool exitedOrKilled() const;

    bool exited() const;

    bool killedBySignal() const;

    int getExitCode() const;

    int getSignal() const;

    std::string getOutput() const;

 private:
    DeathStatus(int _exitCode, int _signal, std::string _output);

    int exitCode;
    int signal;
    std::string output;

friend DeathStatus checkDeath(const std::function<void()>& func);
friend std::ostream& operator<<(std::ostream& out, const DeathStatus& status);
};

}
}

#endif
