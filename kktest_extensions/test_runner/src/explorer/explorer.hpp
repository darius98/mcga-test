#ifndef KKTEST_EXT_TEST_RUNNER_EXPLORER_EXPLORER_H_
#define KKTEST_EXT_TEST_RUNNER_EXPLORER_EXPLORER_H_

#include <functional>

#include <fsystem>

namespace kktest {
namespace test_runner {

void explore(const fsystem::Path& startPath, const std::function<void(fsystem::File)>& onTestFound);

}
}

#endif
