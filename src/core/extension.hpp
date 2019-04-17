#pragma once

#include <mcga/cli.hpp>

#include "hooks_manager.hpp"

namespace mcga::test {

class Extension {
  public:
    virtual ~Extension() = default;

    virtual void registerCommandLineArgs(mcga::cli::Parser&) {
    }

    virtual void init(HooksManager&) {
    }

    virtual void destroy() {
    }
};

}  // namespace mcga::test
