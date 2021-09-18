#include "time_tick.hpp"

namespace mcga::test {

std::chrono::nanoseconds ComputeTimeTickLengthFromHardware() {
    // TODO(darius98): Don't hard-code this!
    return std::chrono::seconds(1);
}

std::chrono::nanoseconds GetTimeTickLength() {
    static const auto timeTickLength = ComputeTimeTickLengthFromHardware();
    return timeTickLength;
}

std::chrono::nanoseconds TimeTicksToNanoseconds(double timeTicks) {
    return std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(
      timeTicks * GetTimeTickLength().count()));
}

}  // namespace mcga::test
