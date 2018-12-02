#ifndef KKTEST_EXTENSIONS_TEST_RUNNER_SRC_EXPLORER_EXPLORER_HPP_
#define KKTEST_EXTENSIONS_TEST_RUNNER_SRC_EXPLORER_EXPLORER_HPP_

#include <functional>

#include <fsystem.hpp>

namespace kktest {
namespace test_runner {

void explore(const fsystem::Path& startPath, const std::function<void(fsystem::File)>& onTestFound);

}  // namespace test_runner
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_TEST_RUNNER_SRC_EXPLORER_EXPLORER_HPP_
