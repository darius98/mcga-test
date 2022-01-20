#pragma once

#include "death_status.hpp"
#include "mcga/test.hpp"

namespace mcga::test::internal {

void check_death(Executable func,
                 double timeTicksLimit,
                 int* exitCode,
                 int* exitSignal);

}

namespace mcga::test {

template<internal::executable_t Callable>
DeathStatus checkDeath(Callable func,
                       double timeTicksLimit = 1,
                       Context context = Context()) {
    int exitCode, exitSignal;
    internal::check_death(Executable(std::move(func), context),
                          timeTicksLimit,
                          &exitCode,
                          &exitSignal);
    return DeathStatus{exitCode, exitSignal};
}

}  // namespace mcga::test
