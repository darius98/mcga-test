#pragma once

#include "mcga/test.hpp"
#include "death_status.hpp"

namespace mcga::test::death {

DeathStatus checkDeath(const Executable& func, double timeTicksLimit = 1);

}
