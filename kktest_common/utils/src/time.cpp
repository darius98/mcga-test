#include <thread>

#include <kktest_common/time.hpp>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::time_point;
using std::this_thread::sleep_for;

namespace kktest {
namespace utils {

Stopwatch::Stopwatch(double _ms):
        ms(_ms), startTime(Clock::now()) {}

bool Stopwatch::isElapsed() const {
    return msSince(startTime) > ms;
}

Timer::Timer(): startTime(Clock::now()) {}

double Timer::getMsElapsed() const {
    return msSince(startTime);
}

double msSince(TimePoint pastMoment) {
    return duration_cast<milliseconds>(Clock::now() - pastMoment).count();
}

void sleepForMs(int ms) {
    sleep_for(milliseconds(ms));
}

}  // namespace utils
}  // namespace kktest
