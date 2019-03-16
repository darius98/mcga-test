#pragma once

#include <chrono>
#include <thread>

namespace kktest::utils {

typedef std::chrono::high_resolution_clock Clock;

typedef Clock::duration Duration;

typedef Clock::time_point Timestamp;

inline Timestamp now() {
    return Clock::now();
}

class Timer {
 public:
    Timer(): startTime(now()) {}

    Duration elapsed() const {
        return now() - startTime;
    }

    long long elapsedNs() const {
        return std::chrono::duration_cast
                <std::chrono::nanoseconds>(elapsed()).count();
    }

 private:
    Timestamp startTime;
};

class Stopwatch {
 public:
    explicit Stopwatch(Duration duration): endTime(now() + duration) {}

    bool isElapsed() const { return endTime < now(); }

 private:
    Timestamp endTime;
};

inline void sleep(Duration duration) {
    std::this_thread::sleep_for(duration);
}

inline void spin(Duration duration) {
    Stopwatch watch(duration);
    volatile int spins = 0;
    while (!watch.isElapsed()) {
        spins += 1;
    }
}

}
