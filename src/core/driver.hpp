#pragma once

#include <thread>

#include "disallow_copy_and_move.hpp"
#include "executor.hpp"
#include "utils.hpp"

namespace mcga::test {

class Driver {
  private:
    static inline Driver* instance = nullptr;

  public:
    static Driver* Instance();

    static void Init(Driver* driver);

    static void Clean();

    Driver() = default;
    MCGA_DISALLOW_COPY_AND_MOVE(Driver);

    [[nodiscard]] Executor::Type getExecutorType() const;

    void setExecutor(Executor* executor);

    void addGroup(GroupConfig config, Executable body);

    void addTest(TestConfig config, Executable body);

    void addSetUp(Executable setUp);

    void addTearDown(Executable tearDown);

    void addFailure(Test::ExecutionInfo info);

    void addCleanup(Executable cleanup);

  private:
    void emitWarning(Warning warning);

    bool checkMainThreadAndInactive(const String& method,
                                    const Context& context);

    Executor* executor = nullptr;

    std::size_t testingThreadId = current_thread_id();
    std::vector<GroupPtr> groupStack = {};
    int currentTestId = 0;
    int currentGroupId = 0;
};

}  // namespace mcga::test
