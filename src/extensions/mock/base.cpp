#define _GNU_SOURCE
#include <dlfcn.h>

#include <map>
#include <stdexcept>

#include "core/export.hpp"

namespace mcga::test::mock::internal {

MCGA_TEST_EXPORT extern "C" void*
  mcga_test_ext_find_next_symbol(const char* name) noexcept {
    return dlsym(RTLD_NEXT, name);
}

MCGA_TEST_EXPORT extern "C" void mcga_test_ext_after_noreturn_invoke() {
    throw std::runtime_error(
      "Replacement for function marked [[noreturn]] returned.");
}

}  // namespace mcga::test::mock::internal
