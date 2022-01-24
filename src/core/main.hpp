#pragma once

#include "executor.hpp"
#include "extension.hpp"

namespace mcga::test {

struct EntryPointOptions {
    Extension* const* extensions = nullptr;
    std::size_t numExtensions = 0;

    int numRuns = 1;
};

void runTests(Executor* runner, const EntryPointOptions& options);

}  // namespace mcga::test
