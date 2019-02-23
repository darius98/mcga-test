#ifndef KKTEST_COMMON_UTILS_SRC_TIME_HPP_
#define KKTEST_COMMON_UTILS_SRC_TIME_HPP_

namespace kktest {
namespace utils {

class Duration {
 public:
    static constexpr int kSecondsToMilli = 1000;
    static constexpr int kMilliToNano = 1000000;
    static constexpr int kSecondsToNano = kSecondsToMilli * kMilliToNano;

    static Duration fromMs(double ms);

    Duration();
    Duration(const Duration& other);
    Duration(Duration&& other) noexcept;
    Duration(int nSeconds, long long nNanoseconds);

    Duration operator+(const Duration& other) const;
    Duration operator-(const Duration& other) const;

    bool operator<(const Duration& other) const;
    bool operator==(const Duration& other) const;

    int getSeconds() const;

    long long getNanoseconds() const;

    long long totalNs() const;

 private:
    void normalize();

    int nSeconds = 0;
    long long nNanoseconds = 0;
};

typedef Duration ProcessTimestamp;

ProcessTimestamp processNow();
ProcessTimestamp realNow();

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

class RealTimeStopwatch {
 public:
    explicit RealTimeStopwatch(Duration duration);

    bool isElapsed() const;

 private:
    ProcessTimestamp endTime;
};

void sleepForDuration(const Duration& duration);

void spinForDuration(const Duration& duration);

}
}

#endif
