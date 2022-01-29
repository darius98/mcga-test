#pragma once

#include <chrono>

namespace mcga::test {

std::chrono::high_resolution_clock::time_point Now();

double TimeTicksSince(std::chrono::high_resolution_clock::time_point startTime);

std::chrono::nanoseconds GetTimeTickLength();

std::chrono::nanoseconds TimeTicksToNanoseconds(double timeTicks);

double NanosecondsToTimeTicks(std::chrono::nanoseconds ns);

}  // namespace mcga::test
