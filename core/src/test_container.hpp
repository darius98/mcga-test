#ifndef KKTEST_CORE_TEST_CONTAINER_HPP_
#define KKTEST_CORE_TEST_CONTAINER_HPP_

#include <kktest_common/interproc.hpp>
#include <kktest_common/string.hpp>
#include <kktest_common/time.hpp>

namespace kktest {

class TestContainer {
 public:
    typedef std::function<void(const interproc::Message&)> Callback;

    TestContainer(double timeLimitMs,
                  interproc::WorkerSubprocess::Work run,
                  Callback _callback);

    bool poll();

 private:
    utils::ProcessStopwatch testProcessStopwatch;
    interproc::WorkerSubprocess testWorker;
    Callback callback;
};

}

#endif
