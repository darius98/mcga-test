#include <cstdlib>
#include <cstring>

#include <mcga/test/config.hpp>

#include "export.hpp"

#if MCGA_TEST_ALLOW_DYNAMIC_MEMORY
namespace mcga::test::internal {

MCGA_TEST_EXPORT const char* duplicate_str(const char* a) {
    const auto len = std::strlen(a);
    const auto dup = static_cast<char*>(std::malloc(len + 1));
    std::strcpy(dup, a);
    return dup;
}

MCGA_TEST_EXPORT const char* duplicate_str(const char* a, const char* b) {
    const auto len = std::strlen(a) + std::strlen(b);
    const auto dup = static_cast<char*>(std::malloc(len + 1));
    std::strcpy(dup, a);
    std::strcat(dup, b);
    return dup;
}

MCGA_TEST_EXPORT const char*
  duplicate_str(const char* a, const char* b, const char* c) {
    const auto len = std::strlen(a) + std::strlen(b) + std::strlen(c);
    const auto dup = static_cast<char*>(std::malloc(len + 1));
    std::strcpy(dup, a);
    std::strcat(dup, b);
    std::strcat(dup, c);
    return dup;
}

MCGA_TEST_EXPORT void delete_str(const char* data) {
    std::free(const_cast<char*>(data));
}

}  // namespace mcga::test::internal
#endif
