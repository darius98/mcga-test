#ifndef KKTEST_RUNNER_EXPLORER_EXPLORER_H_
#define KKTEST_RUNNER_EXPLORER_EXPLORER_H_

#include <functional>

#include <fsystem>

namespace runner {

void explore(fsystem::Path startPath, const std::function<void(fsystem::File)>& onTestFound);

}

#endif
