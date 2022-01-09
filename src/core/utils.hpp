#pragma once

#include <thread>

namespace mcga::test {

inline auto current_thread_id() {
    return std::hash<std::thread::id>()(std::this_thread::get_id());
}

}  // namespace mcga::test
