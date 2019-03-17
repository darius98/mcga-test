#pragma once

#include <chrono>

namespace kktest {

std::chrono::nanoseconds getTimeTickLength();

std::chrono::nanoseconds timeTicksToNanoseconds(double timeTicks);

}
