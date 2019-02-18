#include "common/utils/src/process_time.hpp"

#include <cmath>

using namespace std;

namespace kktest {
namespace utils {

Duration Duration::seconds(double seconds) {
    return Duration(
        static_cast<int>(floor(seconds)),
        static_cast<long long>((seconds - floor(seconds)) * kSecondsToNano));
}

Duration Duration::milliseconds(double milliseconds) {
    return Duration(
        static_cast<int>(floor(milliseconds / kSecondsToMilli)),
        static_cast<long long>(
            (milliseconds
             - floor(milliseconds / kSecondsToMilli) * kSecondsToMilli
             ) * kMilliToNano));
}

Duration Duration::nanoseconds(long long nanoseconds) {
    return Duration(
        static_cast<int>(nanoseconds / kSecondsToNano),
        static_cast<int>(
            nanoseconds
            - static_cast<int>(nanoseconds / kSecondsToNano) * kSecondsToNano));
}

Duration::Duration(): nSeconds(0), nNanoseconds(0) {}

Duration::Duration(int _nSeconds, long long _nNanoseconds):
        nSeconds(_nSeconds), nNanoseconds(_nNanoseconds) {
    if (nNanoseconds >= kSecondsToNano || nNanoseconds <= -kSecondsToNano) {
        nSeconds += nNanoseconds / kSecondsToNano;
        nNanoseconds %= kSecondsToNano;
    }
    if (nNanoseconds < 0) {
        nNanoseconds += kSecondsToNano;
        nSeconds -= 1;
    }
}

Duration::Duration(const Duration& other) = default;

Duration Duration::operator+(const Duration& other) const {
    Duration result(nSeconds + other.nSeconds,
                    nNanoseconds + other.nNanoseconds);
    if (result.nNanoseconds >= kSecondsToNano) {
        result.nSeconds += 1;
        result.nNanoseconds -= 1;
    }
    return result;
}

Duration Duration::operator-(const Duration& other) const {
    Duration result(nSeconds - other.nSeconds,
                    nNanoseconds - other.nNanoseconds);
    if (result.nNanoseconds < 0) {
        result.nNanoseconds += kSecondsToNano;
        result.nSeconds -= 1;
    }
    return result;
}

bool Duration::operator<(const Duration& other) const {
    return nSeconds < other.nSeconds
           || (nSeconds == other.nSeconds && nNanoseconds < other.nNanoseconds);
}

bool Duration::operator==(const Duration& other) const {
    return nSeconds == other.nSeconds && nNanoseconds == other.nNanoseconds;
}

long long Duration::totalNs() const {
    return 1LL * nSeconds * kSecondsToNano + nNanoseconds;
}

int Duration::getSeconds() const {
    return nSeconds;
}

long long Duration::getNanoseconds() const {
    return nNanoseconds;
}

ProcessStopwatch::ProcessStopwatch(Duration duration):
        endTime(processNow() + duration) {}

bool ProcessStopwatch::isElapsed() const {
    return endTime < processNow();
}

ProcessTimer::ProcessTimer(): startTime(processNow()) {}

Duration ProcessTimer::elapsed() const {
    return processNow() - startTime;
}

void spinForDuration(const Duration& duration) {
    auto end = processNow() + duration;
    volatile int spins = 0;
    while (processNow() < end) {
        spins += 1;
    }
}

}
}
