#pragma once

#include <chrono>

namespace kktest {

std::chrono::nanoseconds GetTimeTickLength();

std::chrono::nanoseconds TimeTicksToNanoseconds(double timeTicks);

}  // namespace kktest
