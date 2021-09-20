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

    virtual void emitWarning(const std::string& message, int groupId);

    std::string stateAsString() const;

    void addFailure(const std::string& failure, Context context);

    Test::ExecutionInfo run(const Test& test);

  private:
    void runJob(const Executable& job,
                Test::ExecutionInfo* execution,
                const std::string& where);

  protected:
    void onWarning(const Warning& warning);
    void onTestExecutionStart(const Test& test);
    void onTestExecutionFinish(const Test& test);

    ExtensionApi* api;

  private:
    enum State { INACTIVE, INSIDE_TEST, INSIDE_SET_UP, INSIDE_TEAR_DOWN };

    State state = INACTIVE;
    int currentTestId = 0;
    std::size_t currentExecutionThreadId = 0;

    std::mutex currentExecutionFailureMutex;
    bool currentExecutionIsFailed = false;
    std::string currentExecutionFailureMessage;
    std::optional<Context> currentExecutionFailureContext;
};

}  // namespace mcga::test
