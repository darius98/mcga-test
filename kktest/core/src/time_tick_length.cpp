#include "kktest/core/include/kktest.hpp"

#include "kktest/core/src/export.hpp"

using namespace std::chrono;

namespace kktest {

namespace {

nanoseconds ComputeTimeTickLengthFromHardware() {
    // TODO(darius98): Don't hard-code this!
    return 1s;
}

}

KKTEST_EXPORT nanoseconds getTimeTickLength() {
    static nanoseconds timeTickLength = 0ns;
    if (timeTickLength == 0ns) {
        timeTickLength = ComputeTimeTickLengthFromHardware();
    }
    return timeTickLength;
}

}
