#ifndef KKTEST_COMMON_UTILS_INCLUDE_KKTEST_COMMON_TIME_HPP_
#define KKTEST_COMMON_UTILS_INCLUDE_KKTEST_COMMON_TIME_HPP_

#include <chrono>

namespace kktest {
namespace utils {

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<Clock> TimePoint;

class Stopwatch {
 public:
    explicit Stopwatch(double _ms);

    bool isElapsed() const;

 private:
    TimePoint startTime;
    double ms;
};

class Timer {
 public:
    Timer();

    double getMillisecondsElapsed() const;

 private:
    TimePoint startTime;
};

double msSince(TimePoint pastMoment);

void sleepForMs(int ms);

}  // namespace utils
}  // namespace kktest

#endif  // KKTEST_COMMON_UTILS_INCLUDE_KKTEST_COMMON_TIME_HPP_
