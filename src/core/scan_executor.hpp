#pragma once

#include <set>

#include "executor.hpp"
#include "extension_api.hpp"

namespace mcga::test {

class ScanExecutor : public Executor {
  public:
    ScanExecutor();

    MCGA_DISALLOW_COPY_AND_MOVE(ScanExecutor);

    ~ScanExecutor() override = default;

    void execute(Test test) override;

  private:
    void discoverGroup(GroupPtr group);

    std::set<int> discoveredGroups;
};

}  // namespace mcga::test
