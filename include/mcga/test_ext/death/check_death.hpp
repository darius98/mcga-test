#pragma once

#include "death_status.hpp"
#include "mcga/test.hpp"

namespace mcga::test::internal {

extern "C" void mcga_test_ext_check_death(Executable func,
                                          double timeTicksLimit,
                                          death::DeathStatus* status);

}

namespace mcga::test::death {

template<internal::executable_t Callable>
DeathStatus checkDeath(Callable func,
                       double timeTicksLimit = 1,
                       Context context = Context()) {
    DeathStatus status;
    internal::mcga_test_ext_check_death(
      Executable(std::move(func), std::move(context)), timeTicksLimit, &status);
    return status;
}

}  // namespace mcga::test::death
