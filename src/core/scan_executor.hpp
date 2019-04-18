#pragma once

#include <set>

#include "executor.hpp"
#include "hooks_manager.hpp"

namespace mcga::test {

class ScanExecutor : public Executor {
  public:
    explicit ScanExecutor(HooksManager* hooks);

    MCGA_DISALLOW_COPY_AND_MOVE(ScanExecutor);

    ~ScanExecutor() override = default;

    void execute(Test test) override;

    void emitWarning(const std::string& message, std::size_t groupId) override;

    Type getType() const override;

  private:
    std::set<int> discoveredGroups;
};

}  // namespace mcga::test
