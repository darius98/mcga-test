#pragma once

#include <chrono>

namespace mcga::test {

std::chrono::nanoseconds GetTimeTickLength();

std::chrono::nanoseconds TimeTicksToNanoseconds(double timeTicks);

}  // namespace mcga::test
