#pragma once

#include <cppli.hpp>

#include "kktest/core/src/hooks_manager.hpp"

namespace kktest {

class Extension {
 public:
    virtual ~Extension() = default;

    virtual void registerCommandLineArgs(cppli::Parser&) {}

    virtual void init(HooksManager&) {}

    virtual void destroy() {}
};

}
