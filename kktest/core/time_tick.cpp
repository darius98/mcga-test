#include "kktest/core/time_tick.hpp"

using namespace std::chrono;

namespace kktest {

nanoseconds ComputeTimeTickLengthFromHardware() {
    // TODO(darius98): Don't hard-code this!
    return 1s;
}

nanoseconds getTimeTickLength() {
    static nanoseconds timeTickLength = 0ns;
    if (timeTickLength == 0ns) {
        timeTickLength = ComputeTimeTickLengthFromHardware();
    }
    return timeTickLength;
}

nanoseconds timeTicksToNanoseconds(double timeTicks) {
    return nanoseconds(
            static_cast<long long>(timeTicks * getTimeTickLength().count()));
}

}
