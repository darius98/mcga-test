#pragma once

#include <vector>

#include "kktest/core/extension.hpp"

namespace kktest {

void runTests(int argc,
              char** argv,
              const std::vector<TestCase*>& tests,
              const std::vector<Extension*>& extensions);

}
