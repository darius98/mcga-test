#pragma once

#include <cstdint>

namespace mcga::test::internal {

// TODO: Remove this once this is available in the standard library
//  implementations.
struct source_location {
    static constexpr source_location
      current(source_location ret
              = source_location(__builtin_LINE(),
                                __builtin_COLUMN(),
                                __builtin_FILE(),
                                __builtin_FUNCTION())) noexcept {
        return ret;
    }

    constexpr source_location(): source_location(0, 0, "", "") {
    }
    constexpr source_location(const source_location&) noexcept = default;
    constexpr source_location(source_location&&) noexcept = default;
    constexpr source_location&
      operator=(const source_location&) noexcept = default;
    constexpr source_location& operator=(source_location&&) noexcept = default;

    [[nodiscard]] constexpr uint_least32_t line() const noexcept {
        return line_;
    }

    [[nodiscard]] constexpr uint_least32_t column() const noexcept {
        return column_;
    }

    [[nodiscard]] constexpr const char* file_name() const noexcept {
        return file_name_;
    }

    [[nodiscard]] constexpr const char* function_name() const noexcept {
        return function_name_;
    }

  private:
    constexpr source_location(uint_least32_t line,
                              uint_least32_t column,
                              const char* file_name,
                              const char* function_name)
            : line_(line), column_(column), file_name_(file_name),
              function_name_(function_name) {
    }

    uint_least32_t line_;
    uint_least32_t column_;
    const char* file_name_;
    const char* function_name_;
};

}  // namespace mcga::internal
