#pragma once

#include <mutex>

#include "disallow_copy_and_move.hpp"
#include "extension_api.hpp"
#include "test.hpp"
#include "warning.hpp"

namespace mcga::test {

struct SynchronizedTestExecution {
    Test::ExecutionInfo info;
    mutable std::mutex infoLock;

    [[nodiscard]] const char* getMessage() const {
        std::lock_guard guard(infoLock);
        return info.message.c_str();
    }

    [[nodiscard]] bool isPassed() const {
        std::lock_guard guard(infoLock);
        return info.isPassed();
    }

    void merge(Test::ExecutionInfo other) {
        std::lock_guard guard(infoLock);
        info.merge(std::move(other));
    }

    void fail(const String& message,
              std::optional<Context> context = std::nullopt) {
        std::lock_guard guard(infoLock);
        info.fail(message, context);
    }

    [[nodiscard]] Test::ExecutionInfo reset() {
        std::lock_guard guard(infoLock);
        Test::ExecutionInfo out = std::move(info);
        info = Test::ExecutionInfo{};
        return out;
    }
};

class Executor {
  public:
    enum Type {
        SCAN,
        SMOOTH,
        BOXED,
    };

    MCGA_DISALLOW_COPY_AND_MOVE(Executor);

    virtual ~Executor() = default;

    void setExtensionApi(ExtensionApi* api);

    [[nodiscard]] ExtensionApi* getExtensionApi() const;

    [[nodiscard]] bool isActive() const;

    [[nodiscard]] Type getType() const;

    virtual void execute(Test test);

    virtual void finalize();

    virtual void emitWarning(Warning warning, GroupPtr group);

    void addFailure(Test::ExecutionInfo info);

    void addCleanup(Executable cleanup);

    Test::ExecutionInfo run(const Test& test);

  protected:
    explicit Executor(Type type);

    void decorateWarningWithCurrentTestNotes(Warning& warning, GroupPtr group);

    void beforeTestExecution(Test& test);

    void onWarning(Warning warning, GroupPtr group);

    ExtensionApi* api;

  private:
    // Note: Returns the last group for which at least one set-up was executed.
    GroupPtr runSetUps(GroupPtr group);

    void runJob(const Executable& job);

    const Type type;

    enum State {
        INACTIVE,
        INSIDE_TEST,
        INSIDE_SET_UP,
        INSIDE_CLEANUP,
        INSIDE_TEAR_DOWN,
    };
    State state = INACTIVE;

    std::size_t currentExecutionThreadId = 0;
    const Executable* currentJob = nullptr;
    const Test* currentTest = nullptr;
    SynchronizedTestExecution currentExecution;

    ExecutableList cleanups;
};

class SmoothExecutor : public Executor {
  public:
    explicit SmoothExecutor();
};

}  // namespace mcga::test
