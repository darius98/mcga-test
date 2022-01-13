#pragma once

#include <mutex>

#include "disallow_copy_and_move.hpp"
#include "extension_api.hpp"
#include "test.hpp"
#include "warning.hpp"

namespace mcga::test {

class Executor {
  public:
    enum Type {
        SCAN,
        SMOOTH,
        BOXED,
    };

    explicit Executor(ExtensionApi* api);

    MCGA_DISALLOW_COPY_AND_MOVE(Executor);

    virtual ~Executor() = default;

    bool isActive() const;

    virtual Type getType() const;

    virtual void execute(Test test);

    virtual void finalize();

    virtual void emitWarning(Warning warning, GroupPtr group);

    std::string stateAsString() const;

    void addFailure(Test::ExecutionInfo info);

    void addCleanup(Executable cleanup);

    Test::ExecutionInfo run(const Test& test);

  private:
    void runJob(const Executable& job, Test::ExecutionInfo* execution);

  protected:
    void decorateWarningWithCurrentTestNotes(Warning& warning, GroupPtr group);

    void addHooksExecutions(Test& test);

    void onWarning(Warning warning, GroupPtr group);
    void onGroupDiscovered(const GroupPtr& group);
    void onTestDiscovered(const Test& test);
    [[nodiscard]] std::optional<Test::ExecutionInfo>
      onTestExecutionStart(const Test& test);
    void onTestExecutionFinish(const Test& test);

    ExtensionApi* api;

    const Executable* currentSetUp = nullptr;
    const Executable* currentTearDown = nullptr;
    const Executable* currentCleanup = nullptr;
    const Test* currentTest = nullptr;

  private:
    enum State { INACTIVE, INSIDE_TEST, INSIDE_SET_UP, INSIDE_TEAR_DOWN };

    State state = INACTIVE;
    std::size_t currentExecutionThreadId = 0;

    std::mutex currentExecutionStatusMutex;
    Test::ExecutionInfo currentExecution;

    std::vector<Executable> currentExecutionCleanups;
};

}  // namespace mcga::test
