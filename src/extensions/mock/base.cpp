#define _GNU_SOURCE
#include <dlfcn.h>

#include <map>
#include <stdexcept>

#include "core/export.hpp"

namespace mcga::test::internal {

MCGA_TEST_EXPORT void* find_next_symbol(const char* name) noexcept {
    return dlsym(RTLD_NEXT, name);
}

MCGA_TEST_EXPORT void after_noreturn_invoke() {
    throw std::runtime_error(
      "Replacement for function marked [[noreturn]] returned.");
}

}  // namespace mcga::test::internal
