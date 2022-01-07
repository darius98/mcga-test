#pragma once

#include <vector>

#include "extension.hpp"

namespace mcga::test {

std::vector<internal::TestCase*> getTestCases();

void runTests(int argc,
              char** argv,
              const std::vector<internal::TestCase*>& tests,
              const std::vector<Extension*>& extensions);

}  // namespace mcga::test
