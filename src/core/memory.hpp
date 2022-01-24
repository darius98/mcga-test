#pragma once

#include "mcga/test.hpp"
#include "intrusive_list.hpp"

namespace mcga::test {

struct GroupAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};

struct ExecutableAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};
using CallbackList = IntrusiveList<Executable, ExecutableAllocator>;

struct WarningNoteAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};

}  // namespace mcga::test
