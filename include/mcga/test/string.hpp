#pragma once

#include <mcga/test/config.hpp>

#include <cstdint>

#if defined(__has_include) && __has_include(<source_location>) && !__clang__
#define MCGA_TEST_USE_SOURCE_LOCATION
#endif

#ifdef MCGA_TEST_USE_SOURCE_LOCATION
#include <source_location>
#endif

namespace mcga::test {

namespace internal {

template<class T, class U>
inline constexpr bool same_as_impl = false;

template<class T>
inline constexpr bool same_as_impl<T, T> = true;

template<class T, class U>
concept same_as = same_as_impl<T, U>;

template<class T>
concept std_string_like = requires(const T& str) {
                              { str.data() } -> same_as<const char*>;
                          };

#if MCGA_TEST_ALLOW_DYNAMIC_MEMORY
const char* duplicate_str(const char* a);
const char* duplicate_str(const char* a, const char* b);
const char* duplicate_str(const char* a, const char* b, const char* c);
void delete_str(const char* data);
#endif

}  // namespace internal

#if MCGA_TEST_ALLOW_DYNAMIC_MEMORY
class String {
    bool isOwned;
    const char* data;

    constexpr String(bool isOwned, const char* data) noexcept
            : isOwned(isOwned), data(data) {
    }

  public:
    static String cat(const char* a, const char* b) {
        return {true, internal::duplicate_str(a, b)};
    }
    static String cat(const char* a, const char* b, const char* c) {
        return {true, internal::duplicate_str(a, b, c)};
    }

    constexpr String() noexcept: String("") {
    }

    constexpr String(const char* data) noexcept: String(false, data) {
    }

    String(const internal::std_string_like auto& data)
            : String(true, internal::duplicate_str(data.data())) {
    }

    constexpr String(const String& other)
            : isOwned(other.isOwned),
              data(isOwned ? internal::duplicate_str(other.data) : other.data) {
    }

    constexpr String(String&& other) noexcept
            : isOwned(other.isOwned), data(other.data) {
        other.isOwned = false;
    }

    constexpr String& operator=(const String& other) {
        if (this != &other) {
            this->~String();
            isOwned = other.isOwned;
            data = isOwned ? internal::duplicate_str(other.data) : other.data;
        }
        return *this;
    }

    constexpr String& operator=(String&& other) noexcept {
        if (this != &other) {
            this->~String();
            isOwned = other.isOwned;
            data = other.data;
            other.isOwned = false;
        }
        return *this;
    }

    constexpr ~String() {
        if (isOwned) {
            internal::delete_str(data);
        }
    }

    [[nodiscard]] constexpr const char* c_str() const noexcept {
        return data;
    }
};
#else
class String {
    const char* data;

  public:
    constexpr String() noexcept: data("") {
    }

    constexpr String(const char* data) noexcept: data(data) {
    }

    String(const String& other) = default;
    String(String&& other) = default;
    String& operator=(const String& other) = default;
    String& operator=(String&& other) = default;

    // Don't allow String to be trivial, even in this case, as it might get
    // wrongly de-serialized by mcga::proc.
    constexpr ~String() {
    }

    [[nodiscard]] constexpr const char* c_str() const noexcept {
        return data;
    }
};
#endif

struct Context {
    String fileName;
    String functionName;
    std::uint_least32_t line;
    std::uint_least32_t column;

#ifdef MCGA_TEST_USE_SOURCE_LOCATION
    constexpr explicit Context(std::source_location loc
                               = std::source_location::current())
            : fileName(loc.file_name()), functionName(loc.function_name()),
              line(loc.line()), column(loc.column()) {
    }
#else
    constexpr explicit Context(const char* fileName = __builtin_FILE(),
                               const char* functionName = __builtin_FUNCTION(),
                               std::uint_least32_t line = __builtin_LINE(),
                               std::uint_least32_t column = __builtin_COLUMN())
            : fileName(fileName), functionName(functionName), line(line),
              column(column) {
    }
#endif
};

}  // namespace mcga::test
