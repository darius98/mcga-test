#pragma once

#include "config.hpp"

#include <cstdint>

#if MCGA_TEST_THREADING
#include <thread>
#endif

namespace mcga::test {

inline auto current_thread_id() {
#if MCGA_TEST_THREADING
    return std::hash<std::thread::id>()(std::this_thread::get_id());
#else
    return std::size_t{0};
#endif
}

}  // namespace mcga::test
