#include <cstddef>
#include <cstdint>

extern "C" void* memcpy(void* raw_dst, const void* raw_src, std::size_t len) {
    auto dst = static_cast<unsigned char*>(raw_dst);
    auto src = static_cast<const unsigned char*>(raw_src);
    while (len != 0) {
        *dst = *src;
        dst++;
        src++;
        len -= 1;
    }
    return dst;
}
