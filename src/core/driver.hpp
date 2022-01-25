#pragma once

#include "disallow_copy_and_move.hpp"
#include "executor.hpp"
#include "utils.hpp"

namespace mcga::test {

class Driver {
    Executor* executor = nullptr;

    std::size_t testingThreadId = current_thread_id();
    GroupPtr currentGroup;
    int currentTestId = 0;
    int currentGroupId = 0;

  public:
    static Driver* Instance();

    static void Init(Driver* driver);

    static void Clean();

    explicit Driver(Executor* executor);
    MCGA_DISALLOW_COPY_AND_MOVE(Driver);

    [[nodiscard]] Executor::Type getExecutorType() const;

    void addGroup(GroupConfig config, Executable body);

    void addTest(TestConfig config, Executable body);

    void addSetUp(Executable setUp);

    void addTearDown(Executable tearDown);

    void addFailure(Test::ExecutionInfo info);

    void addCleanup(Executable cleanup);

  private:
    void emitWarning(Warning warning);

    bool checkMainThreadAndInactive(WarningNoteType method,
                                    const Context& context);
};

}  // namespace mcga::test
