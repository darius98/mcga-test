#pragma once

#include <kktest_ext/death_impl/death_status.hpp>

namespace kktest::death {

KKTEST_EXPORT DeathStatus checkDeath(const Executable& func,
                                     double timeTicksLimit = 1);

}
