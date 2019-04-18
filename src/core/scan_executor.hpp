#pragma once

#include "executor.hpp"

namespace mcga::test {

class ScanExecutor : public Executor {
  public:
    void execute(Test test) override;

    Type getType() const override;
};

}  // namespace mcga::test
