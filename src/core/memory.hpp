#pragma once

#include "aa_list.hpp"
#include "mcga/test.hpp"
#include "test_case.hpp"

namespace mcga::test {

struct TestCaseAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};
using TestCaseList = List<TestCase, TestCaseAllocator>;

struct GroupAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};

struct ExecutableAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};
using ExecutableList = List<Executable, ExecutableAllocator>;

struct WarningNoteAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};

}  // namespace mcga::test
