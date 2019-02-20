#include "common/utils/src/process_time.hpp"

#include <cmath>

using namespace std;

namespace kktest {
namespace utils {

Duration Duration::fromMs(double ms) {
    return {
        static_cast<int>(floor(ms / kSecondsToMilli)),
        static_cast<long long>(
            (ms - floor(ms / kSecondsToMilli) * kSecondsToMilli) * kMilliToNano)
    };
}

Duration::Duration() = default;
Duration::Duration(const Duration& other) = default;
Duration::Duration(Duration&& other) noexcept = default;
Duration::Duration(int nSeconds, long long nNanoseconds):
        nSeconds(nSeconds), nNanoseconds(nNanoseconds) {
    normalize();
}

Duration Duration::operator+(const Duration& other) const {
    Duration result(nSeconds + other.nSeconds,
                    nNanoseconds + other.nNanoseconds);
    result.normalize();
    return result;
}

Duration Duration::operator-(const Duration& other) const {
    Duration result(nSeconds - other.nSeconds,
                    nNanoseconds - other.nNanoseconds);
    result.normalize();
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

void Duration::normalize() {
    if (nNanoseconds >= kSecondsToNano || nNanoseconds <= -kSecondsToNano) {
        nSeconds += nNanoseconds / kSecondsToNano;
        nNanoseconds %= kSecondsToNano;
    }
    if (nNanoseconds < 0) {
        nNanoseconds += kSecondsToNano;
        nSeconds -= 1;
    }
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
