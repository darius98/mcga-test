#pragma once

#include <vector>

#include "kktest/core/src/extension.hpp"

namespace kktest {

void initialize(int argc, char** argv, std::vector<Extension*>* extensions);

int runTests(std::vector<TestCase> tests, std::vector<Extension*>* extensions);

}
