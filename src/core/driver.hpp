#pragma once

#include "executor.hpp"
#include "utils.hpp"

namespace mcga::test {

class Driver {
    Executor* executor = nullptr;

    std::size_t testingThreadId = current_thread_id();
    GroupPtr currentGroup;
    int currentTestId = 0;
    int currentGroupId = 0;
    ExtensionApi* api;

  public:
    static Driver* Instance();

    static void Init(Driver* driver);

    static void Clean();

    explicit Driver(ExtensionApi* api, Executor* executor);
    Driver(const Driver&) = delete;
    Driver(Driver&&) = delete;
    Driver& operator=(const Driver&) = delete;
    Driver& operator=(Driver&&) = delete;

    [[nodiscard]] Executor::Type getExecutorType() const;

    void addGroup(GroupConfig config, Executable body);

    void addTest(TestConfig config, Executable body);

    void addSetUp(Executable setUp);

    void addTearDown(Executable tearDown);

    void addFailure(Test::ExecutionInfo info);

    void addCleanup(Executable cleanup);

    void emitWarning(String message, std::optional<Context> context = {});

  private:
    bool checkMainThreadAndInactive(WarningNoteType method,
                                    const Context& context);
};

}  // namespace mcga::test
