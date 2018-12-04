#ifndef KKTEST_CORE_SRC_TEST_CONTAINER_HPP_
#define KKTEST_CORE_SRC_TEST_CONTAINER_HPP_

#include <chrono>

#include <kktest_common/interproc.hpp>
#include <kktest_impl/types.hpp>
#include "test.hpp"

namespace kktest {

class TestContainer {
 public:
    typedef std::function<void(const interproc::Message&)> Callback;
    typedef std::function<void(interproc::PipeWriter*)> SubprocessFunc;

    TestContainer(double _timeLimitMs, SubprocessFunc testFunc, Callback _callback);

    bool poll();

 private:
    bool finish(const String& failureMessage);

    bool finish(const interproc::Message& message);

    bool killTestProcess();

    double timeLimitMs;
    interproc::PipeReader* pipe;
    interproc::SubprocessHandler* testProcess;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    Callback callback;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_TEST_CONTAINER_HPP_
