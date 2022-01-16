#pragma once

#include <vector>

#include "extension.hpp"

namespace mcga::test {

TestCase* getTestCasesListHead();

void runTests(int argc,
              char** argv,
              TestCase* testCasesListHead,
              const std::vector<Extension*>& extensions);

}  // namespace mcga::test
