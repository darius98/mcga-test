#pragma once

#include <memory>
#include <vector>

#include "mcga/proc.hpp"

#include "core/disallow_copy_and_move.hpp"
#include "core/executor.hpp"

namespace mcga::test {

class BoxExecutor : public Executor {
  private:
    using Box = std::pair<Test, std::unique_ptr<proc::WorkerSubprocess>>;

    static constexpr auto loopSleepTime = std::chrono::milliseconds(5);

  public:
    explicit BoxExecutor(std::size_t numBoxes);

    MCGA_DISALLOW_COPY_AND_MOVE(BoxExecutor);

    void execute(Test test) override;

    void finalize() override;

    void emitWarning(Warning warning, GroupPtr group) override;

  private:
    void executeBoxed(const Test& test, std::unique_ptr<proc::PipeWriter> pipe);

    void ensureEmptyBoxes(std::size_t numContainers);

    std::unique_ptr<proc::WorkerSubprocess> startExecution(const Test& test);

    bool tryCloseBox(Box* box);

    std::unique_ptr<proc::PipeWriter> currentTestingSubprocessPipe;
    std::size_t numBoxes;
    std::vector<Box> activeBoxes;
};

}  // namespace mcga::test
