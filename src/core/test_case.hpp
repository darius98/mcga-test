#pragma once

#include <mcga/test/string.hpp>

#include "aa_list.hpp"

namespace mcga::test {

struct TestCase {
    const char* name{};
    void (*body)(){};
    Context context;
};

struct TestCaseAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};

using TestCaseList = List<TestCase, TestCaseAllocator>;

}  // namespace mcga::test
