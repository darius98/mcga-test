#pragma once

#include <vector>

#include "extension.hpp"

namespace mcga::test {

void runTests(int argc,
              char** argv,
              const std::vector<internal::TestCase*>& tests,
              const std::vector<Extension*>& extensions);

}  // namespace mcga::test
