#include <cstdint>

#include "core/export.hpp"

extern "C" {
MCGA_TEST_EXPORT uintptr_t __stack_chk_guard = 0xdeadbeef;
MCGA_TEST_EXPORT void __stack_chk_fail() {
    // TODO: abort.
}
}
