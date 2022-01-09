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

    explicit Driver(Executor* executor);

    MCGA_DISALLOW_COPY_AND_MOVE(Driver);

    ~Driver() = default;

    [[nodiscard]] Executor::Type getExecutorType() const;

    virtual void addGroup(GroupConfig config, Executable body);

    virtual void addTest(TestConfig config, Executable body);

    virtual void addSetUp(Executable setUp);

    virtual void addTearDown(Executable tearDown);

    virtual void addFailure(String failure, Context context);

    virtual void addCleanup(Executable cleanup);

  private:
    void emitWarning(Warning warning);

    bool checkMainThreadAndInactive(const String& method,
                                    const Context& context);

    Executor* executor;

    std::size_t testingThreadId = current_thread_id();
    std::vector<GroupPtr> groupStack = {};
    int currentTestId = 0;
    int currentGroupId = 0;
};

}  // namespace mcga::test
