#include "time_tick.hpp"

#include "config.hpp"

static auto timeTickLength = std::chrono::nanoseconds{0};

namespace mcga::test {

std::chrono::high_resolution_clock::time_point Now() {
#if MCGA_TEST_TIMING
    return std::chrono::high_resolution_clock::now();
#else
    return std::chrono::high_resolution_clock::time_point{};
#endif
}

double
  TimeTicksSince(std::chrono::high_resolution_clock::time_point startTime) {
#if MCGA_TEST_TIMING
    return NanosecondsToTimeTicks(std::chrono::high_resolution_clock::now()
                                  - startTime);
#else
    return -1;
#endif
}

std::chrono::nanoseconds ComputeTimeTickLengthFromHardware() {
    // TODO: Don't hard-code this!
    return std::chrono::seconds(1);
}

std::chrono::nanoseconds GetTimeTickLength() {
    if (timeTickLength.count() == 0) {
        timeTickLength = ComputeTimeTickLengthFromHardware();
    }
    return timeTickLength;
}

std::chrono::nanoseconds TimeTicksToNanoseconds(double timeTicks) {
    return std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(
      timeTicks * GetTimeTickLength().count()));
}

double NanosecondsToTimeTicks(std::chrono::nanoseconds ns) {
    return 1.0 * ns.count() / GetTimeTickLength().count();
}

}  // namespace mcga::test
