#include "common/utils/src/time.hpp"

#include <ctime>

using namespace std;

namespace kktest {
namespace utils {

Timestamp realNow() {
    timespec now{};
    clock_gettime(CLOCK_REALTIME, &now);
    return {static_cast<int>(now.tv_sec), now.tv_nsec};
}

void sleepForDuration(const Duration& duration) {
    timespec sleepDuration{duration.getSeconds(), duration.getNanoseconds()};
    do {
        timespec remainder{};
        int status = nanosleep(&sleepDuration, &remainder);
        if (status == 0) {
            break;
        }
        sleepDuration = remainder;
    } while (true);
}

}
}
