#pragma once

#include <mcga/cli.hpp>

#include "kktest/core/hooks_manager.hpp"

namespace kktest {

class Extension {
 public:
    virtual ~Extension() = default;

    virtual void registerCommandLineArgs(mcga::cli::Parser&) {}

    virtual void init(HooksManager&) {}

    virtual void destroy() {}
};

}
