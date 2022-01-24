#pragma once

#include "disallow_copy_and_move.hpp"
#include "extension_api.hpp"

namespace mcga::cli {

// Forward declare this, so projects that don't want to or can't use traditional
// command line arguments, or simply don't want to pull in the dependency on
// mcga::cli are free to do so, by overwriting the entry point of the library.
struct Parser;

}

namespace mcga::test {

class Extension {
  public:
    Extension() = default;

    MCGA_DISALLOW_COPY_AND_MOVE(Extension);

    virtual ~Extension() = default;

    virtual void registerCommandLineArgs(mcga::cli::Parser* /*parser*/) {
    }

    virtual void init(ExtensionApi* /*api*/) {
    }

    virtual void destroy() {
    }
};

}  // namespace mcga::test
