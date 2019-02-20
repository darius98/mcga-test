#include "common/utils/src/process_time.hpp"

#include <ctime>

using namespace std;

namespace kktest {
namespace utils {

ProcessTimestamp processNow() {
    timespec now{};
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
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
