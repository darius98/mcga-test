#ifndef KKTEST_CORE_SRC_TEST_CONTAINER_HPP_
#define KKTEST_CORE_SRC_TEST_CONTAINER_HPP_

#include <kktest_common/interproc.hpp>
#include <kktest_common/time.hpp>
#include <kktest_impl/types.hpp>

namespace kktest {

class TestContainer {
 public:
    typedef std::function<void(const interproc::Message&)> Callback;

    TestContainer(double timeLimitMs, interproc::SubprocessWork run, Callback _callback);

    bool poll();

 private:
    void finishWithError(const String& failureMessage);

    utils::ProcessStopwatch testProcessStopwatch;
    interproc::WorkerSubprocess testWorker;
    Callback callback;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_TEST_CONTAINER_HPP_
