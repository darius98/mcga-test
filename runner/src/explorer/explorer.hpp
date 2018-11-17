#ifndef KKTEST_RUNNER_EXPLORER_EXPLORER_H_
#define KKTEST_RUNNER_EXPLORER_EXPLORER_H_

#include <fsystem>

namespace runner {

void explore(const std::function<void(fsystem::File)>& onTestFound);

}

#endif
