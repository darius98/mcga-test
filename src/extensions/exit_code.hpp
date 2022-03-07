#pragma once

#include "core/extension.hpp"

namespace mcga::test {

class ExitCodeExtension {
  public:
    explicit ExitCodeExtension(bool skipIsFail, bool ignoreWarnings);

    [[nodiscard]] int getExitCode() const;

    void afterTestExecution(const Test& test);

    void onWarning(const Warning&);

    void destroy();

  private:
    int exitCode;
    bool skippedAnyTests = false;
    bool passedAnyTests = false;

    bool skipIsFail;
    bool ignoreWarnings;
};

}  // namespace mcga::test