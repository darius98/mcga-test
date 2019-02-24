#ifndef KKTEST_COMMON_UTILS_SRC_TIME_HPP_
#define KKTEST_COMMON_UTILS_SRC_TIME_HPP_

namespace kktest {
namespace utils {

class Duration {
 public:
    static constexpr int kSecondsToMilli = 1000;
    static constexpr int kMilliToNano = 1000000;
    static constexpr int kSecondsToNano = kSecondsToMilli * kMilliToNano;

    static Duration fromMs(long double ms);

    Duration() = default;
    Duration(const Duration& other) = default;
    Duration(Duration&& other) noexcept = default;
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

Duration operator"" _ms(long double ms);
Duration operator"" _ms(unsigned long long ms);
Duration operator"" _s(long double s);
Duration operator"" _s(unsigned long long s);
Duration operator"" _ns(unsigned long long ns);

typedef Duration Timestamp;

Timestamp realNow();

class RealTimeTimer {
 public:
    RealTimeTimer();

    Duration elapsed() const;

 private:
    Timestamp startTime;
};

class RealTimeStopwatch {
 public:
    explicit RealTimeStopwatch(Duration duration);

    bool isElapsed() const;

 private:
    Timestamp endTime;
};

void sleepForDuration(const Duration& duration);

void spinForDuration(const Duration& duration);

}
}

#endif
