#pragma once

#include <mcga/test/string.hpp>

namespace mcga::test {

struct TestCase {
    const char* name{};
    void (*body)(){};
    Context context;
};

}  // namespace mcga::test
