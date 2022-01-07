#define _GNU_SOURCE
#include <dlfcn.h>

#include <map>
#include <stdexcept>

#include "core/export.hpp"

// Use a map and not a vector so multiple replacements for the same function in
// the same test only get cleaned up once.
static std::map<void*, void (*)(void*)> after_test_cleanups;

namespace mcga::test::mock::internal {

MCGA_TEST_EXPORT void* find_next_symbol(const char* name) noexcept {
    return dlsym(RTLD_NEXT, name);
}

MCGA_TEST_EXPORT void after_noreturn_invoke() {
    throw std::runtime_error(
      "Replacement for function marked [[noreturn]] returned.");
}

MCGA_TEST_EXPORT void add_after_test_cleanup(void (*cb)(void*), void* data) {
    after_test_cleanups.emplace(data, cb);
}

// Intentionally not exported (only called internally).
void run_after_test_cleanups() {
    for (const auto& cb_and_data: after_test_cleanups) {
        cb_and_data.second(cb_and_data.first);
    }
    after_test_cleanups.clear();
}

}  // namespace mcga::test::mock::internal
