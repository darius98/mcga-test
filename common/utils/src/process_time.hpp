#ifndef KKTEST_COMMON_UTILS_SRC_TIME_HPP_
#define KKTEST_COMMON_UTILS_SRC_TIME_HPP_

namespace kktest {
namespace utils {

class Duration {
 public:
    static constexpr int kSecondsToMilli = 1000;
    static constexpr int kMilliToNano = 1000000;
    static constexpr int kSecondsToNano = kSecondsToMilli * kMilliToNano;

    static Duration seconds(double seconds);
    static Duration milliseconds(double milliseconds);
    static Duration nanoseconds(long long nanoseconds);

    Duration();
    Duration(int _nSeconds, long long _nNanoseconds);
    Duration(const Duration& other);

    Duration operator+(const Duration& other) const;
    Duration operator-(const Duration& other) const;

    bool operator<(const Duration& other) const;
    bool operator==(const Duration& other) const;

    long long totalNs() const;

    int getSeconds() const;

    long long getNanoseconds() const;

 private:
    int nSeconds;
    long long nNanoseconds;
};

typedef Duration ProcessTimestamp;

ProcessTimestamp processNow();

class ProcessStopwatch {
 public:
    explicit ProcessStopwatch(Duration duration);

    bool isElapsed() const;

 private:
    ProcessTimestamp endTime;
};

class ProcessTimer {
 public:
    ProcessTimer();

    Duration elapsed() const;

 private:
    ProcessTimestamp startTime;
};

void sleepForDuration(const Duration& duration);

void spinForDuration(const Duration& duration);

}
}

#endif
