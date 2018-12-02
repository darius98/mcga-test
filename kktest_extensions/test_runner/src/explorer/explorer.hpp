#ifndef KKTEST_EXTENSIONS_TEST_RUNNER_SRC_EXPLORER_EXPLORER_HPP_
#define KKTEST_EXTENSIONS_TEST_RUNNER_SRC_EXPLORER_EXPLORER_HPP_

#include <filesystem>
#include <functional>

namespace kktest {
namespace test_runner {

void explore(const std::filesystem::path& startPath,
             const std::function<void(std::filesystem::path)>& onTestFound);

}  // namespace test_runner
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_TEST_RUNNER_SRC_EXPLORER_EXPLORER_HPP_
