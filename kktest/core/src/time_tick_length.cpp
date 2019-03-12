#include "kktest/core/include/kktest.hpp"

#include "kktest/core/src/export.hpp"

namespace kktest {

namespace {

double ComputeTimeTickLengthFromHardware() {
    // TODO(darius98): Don't hard-code this!
    return 1000;
}

}

KKTEST_EXPORT double getTimeTickLengthMs() {
    static double timeTickLengthMs = -1;
    if (timeTickLengthMs == -1) {
        timeTickLengthMs = ComputeTimeTickLengthFromHardware();
    }
    return timeTickLengthMs;
}

}