#include "time_tick.hpp"

namespace mcga::test {

std::chrono::nanoseconds ComputeTimeTickLengthFromHardware() {
    // TODO: Don't hard-code this!
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

double NanosecondsToTimeTicks(std::chrono::nanoseconds ns) {
    return 1.0 * ns.count() / GetTimeTickLength().count();
}

}  // namespace mcga::test
