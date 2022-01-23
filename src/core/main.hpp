#pragma once

#include <vector>

#include "extension.hpp"

namespace mcga::test {

TestCase* getTestCasesListHead();

void runTests(int argc,
              char** argv,
              const std::vector<Extension*>& extensions);

}  // namespace mcga::test
