#include "kktest/core/include/kktest.hpp"

#include "kktest/core/export.hpp"

using namespace std::chrono;

namespace kktest {

nanoseconds ComputeTimeTickLengthFromHardware() {
    // TODO(darius98): Don't hard-code this!
    return 1s;
}

KKTEST_EXPORT nanoseconds getTimeTickLength() {
    static nanoseconds timeTickLength = 0ns;
    if (timeTickLength == 0ns) {
        timeTickLength = ComputeTimeTickLengthFromHardware();
    }
    return timeTickLength;
}

}
