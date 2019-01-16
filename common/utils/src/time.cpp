#include <chrono>
#include <thread>

#include <kktest_common/time.hpp>

using namespace std::chrono;
using namespace std::this_thread;

namespace {

timespec processNow() {
    timespec t{};
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    return t;
}

double msSince(timespec pastMoment) {
    timespec now = processNow();
    return (now.tv_sec - pastMoment.tv_sec) * 1000.0
            + (now.tv_nsec - pastMoment.tv_nsec) * 0.000001;
}

}  // namespace

namespace kktest {
namespace utils {

ProcessStopwatch::ProcessStopwatch(double _ms):
        ms(_ms), startTime(processNow()) {}

bool ProcessStopwatch::isElapsed() const {
    return msSince(startTime) > ms;
}

ProcessTimer::ProcessTimer(): startTime(processNow()) {}

double ProcessTimer::getMsElapsed() const {
    return msSince(startTime);
}

void sleepForMs(int ms) {
    sleep_for(milliseconds(ms));
}

void spinForMs(double ms) {
    timespec start = processNow();
    volatile int spins = 0;
    while (msSince(start) < ms) {
        spins += 1;
    }
}

}
}
