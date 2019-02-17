#ifndef KKTEST_CORE_SRC_TEST_CONTAINER_HPP_
#define KKTEST_CORE_SRC_TEST_CONTAINER_HPP_

#include "common/interproc/src/subprocess.hpp"
#include "common/string/src/string.hpp"
#include "common/utils/src/time.hpp"

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
