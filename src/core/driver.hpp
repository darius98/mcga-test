#pragma once

#include <thread>

#include "disallow_copy_and_move.hpp"
#include "executor.hpp"

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

    virtual void
      addGroup(GroupConfig config, const Executable& body, Context context);

    virtual void addTest(TestConfig config, Executable body, Context context);

    virtual void addSetUp(UserTestExecutable setUp);

    virtual void addTearDown(UserTestExecutable tearDown);

    virtual void addFailure(const std::string& failure, Context context);

  private:
    void emitWarning(Warning warning);

    bool checkMainThreadAndInactive(const std::string& method,
                                    const Context& context);

    Executor* executor = nullptr;

    std::size_t testingThreadId
      = std::hash<std::thread::id>()(std::this_thread::get_id());
    std::vector<GroupPtr> groupStack = {};
    int currentTestId = 0;
    int currentGroupId = 0;
};

}  // namespace mcga::test
