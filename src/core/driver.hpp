#pragma once

#include "disallow_copy_and_move.hpp"
#include "executor.hpp"
#include "hooks_manager.hpp"

namespace mcga::test {

class Driver : public HooksManager {
  private:
    static inline Driver* instance = nullptr;

  public:
    static Driver* Instance();

    static void Init(Driver* driver);

    Driver(const HooksManager& hooks, Executor* executor);

    MCGA_DISALLOW_COPY_AND_MOVE(Driver);

    ~Driver() override;

    Executor::Type getExecutorType() const;

    void addGroup(GroupConfig config, const Executable& body);

    void addTest(TestConfig config, Executable body);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

    void addFailure(const std::string& failure);

    void emitWarning(const std::string& message);

  private:
    bool checkMainThreadAndInactive(const std::string& method);

    void onWarning(const Warning& warning);
    void beforeTest(const Test& test);
    void afterTest(const ExecutedTest& test);
    void beforeGroup(const GroupPtr& group);
    void afterGroup(const GroupPtr& group);

    std::size_t testingThreadId;
    Executor* executor;
    std::vector<GroupPtr> groupStack = {};
    int currentTestId = 0;
    int currentGroupId = 0;
};

}  // namespace mcga::test
