#pragma once

#include <cstdlib>

#include "core/extension.hpp"

namespace mcga::test {

class ExitCodeExtension {
  public:
    explicit ExitCodeExtension(bool skipIsFail, bool ignoreWarnings);

    [[nodiscard]] int getExitCode() const;

    void init(ExtensionApi* api);

    void destroy();

  private:
    int exitCode = EXIT_SUCCESS;
    bool skippedAnyTests = false;
    bool passedAnyTests = false;

    bool skipIsFail;
    bool ignoreWarnings;
};

}