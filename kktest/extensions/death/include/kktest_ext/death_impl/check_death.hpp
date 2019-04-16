#pragma once

#include <kktest.hpp>
#include <kktest_ext/death_impl/death_status.hpp>

namespace mcga::test::death {

DeathStatus checkDeath(const Executable& func, double timeTicksLimit = 1);

}
