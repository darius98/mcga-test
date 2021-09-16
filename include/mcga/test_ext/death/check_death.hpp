#pragma once

#include "death_status.hpp"
#include "mcga/test.hpp"

namespace mcga::test::death {

DeathStatus checkDeath(const Executable& func, double timeTicksLimit = 1);

}
