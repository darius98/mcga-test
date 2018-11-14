#ifndef KKTEST_RUNNER_EXPLORER_EXPLORER_H_
#define KKTEST_RUNNER_EXPLORER_EXPLORER_H_

#include <os/file.hpp>


namespace kktest_runner {

void explore(const std::function<void(File)>& onTestFound);

}

#endif
