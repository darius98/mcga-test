#include "time_tick_length.hpp"

namespace kktest {
namespace utils {

double computeTimeTickLengthFromHardware() {
     // TODO: Don't hardcode this, estimate it based on how much a series of computations takes.
    return 1000.0;
}

}
}
