#ifndef KKTEST_EXTENSIONS_DEATH_KKTEST_EXT_DEATH_IMPL_DEATH_STATUS_HPP_
#define KKTEST_EXTENSIONS_DEATH_KKTEST_EXT_DEATH_IMPL_DEATH_STATUS_HPP_

#include <kktest_common/string.hpp>
#include <kktest_impl/executable.hpp>

namespace kktest {
namespace death {

class DeathStatus {
 public:
    DeathStatus();

    bool exitedOrKilled() const;

    bool exited() const;

    bool killedBySignal() const;

    int getExitCode() const;

    int getSignal() const;

    std::string getOutput() const;

 private:
    DeathStatus(int _exitCode, int _signal, String _output);

    int exitCode;
    int signal;
    String output;

friend DeathStatus checkDeath(const std::function<void()>& func);
};

}
}

#endif
