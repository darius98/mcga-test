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

    Type getType() const override;

  private:
    HooksManager* hooks;
    std::set<int> discoveredGroups;
};

}  // namespace mcga::test
