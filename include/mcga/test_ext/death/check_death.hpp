#pragma once

#include "death_status.hpp"
#include "mcga/test.hpp"

namespace mcga::test {
inline namespace MCGA_TEST_ABI_NS {

DeathStatus checkDeath(Executable func, double timeTicksLimit = 1);

}  // namespace MCGA_TEST_ABI_NS
}  // namespace mcga::test
