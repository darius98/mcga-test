#pragma once

#include <cstdarg>
#include <memory>
#include <type_traits>

#include <mcga/test.hpp>

namespace mcga::test {

namespace internal {

template<class T, class R, class... Args>
concept invocable_with = requires {
    std::is_invocable_r_v<R, T, Args...>;
};

template<int i, class... Args>
struct nth;

template<class H, class... T>
struct nth<0, H, T...> {
    using type = H;
};

template<int i, class H, class... T>
struct nth<i, H, T...> : nth<i - 1, T...> {};

void* find_next_symbol(const char* name) noexcept;

[[noreturn]] void after_noreturn_invoke();

}  // namespace internal

template<class F>
class BaseFunctionMock;

template<class R, class... Args>
class BaseFunctionMock<R(Args...)> {
  public:
    using return_t = R;

    template<int i>
    using arg_t = typename internal::nth<i, Args...>::type;

    using F = R(Args...);

  protected:
    void* replacementData = nullptr;
    R (*replacement)(void*, Args...) = nullptr;
    void (*replacementDtor)(void*) = nullptr;

  public:
    constexpr BaseFunctionMock() = default;

    ~BaseFunctionMock() {
        reset();
    }

    void reset() {
        if (is_replaced()) {
            replacementDtor(replacementData);
        }
        replacementData = nullptr;
        replacement = nullptr;
        replacementDtor = nullptr;
    }

    [[nodiscard]] constexpr bool is_replaced() const {
        return replacement != nullptr;
    }

    template<internal::invocable_with<R, Args...> Callable>
    void replace(Callable callable) {
        replacementData = new Callable(std::move(callable));
        replacement = [](void* data, Args... args) -> R {
            return (*static_cast<Callable*>(data))(args...);
        };
        replacementDtor = [](void* data) {
            delete static_cast<Callable*>(data);
        };
        cleanup([this] {
            reset();
        });
    }
};

template<class F>
class FunctionMock;

template<class R, class... Args>
class FunctionMock<R(Args...)> : public BaseFunctionMock<R(Args...)> {
    using F = typename BaseFunctionMock<R(Args...)>::F;

    const char* symbol;
    F* original = nullptr;

  public:
    explicit constexpr FunctionMock(const char* symbol) noexcept
            : symbol(symbol) {
    }

    R invoke(Args... args) {
        if (this->replacement != nullptr) {
            return this->replacement(this->replacementData, args...);
        } else {
            if (original == nullptr) {
                original
                  = reinterpret_cast<F*>(internal::find_next_symbol(symbol));
            }
            return original(args...);
        }
    }
};

template<class R, class... Args>
class FunctionMock<R(Args..., ...)>
        : public BaseFunctionMock<R(Args..., va_list)> {
    using VAFunctionMock = FunctionMock<R(Args..., va_list)>;

    VAFunctionMock* va_function_mock;

  public:
    constexpr explicit FunctionMock(VAFunctionMock* va_function_mock) noexcept
            : va_function_mock(va_function_mock) {
    }

    R invoke(Args... args, va_list va_args) {
        if (this->replacement != nullptr) {
            return this->replacement(this->replacementData, args..., va_args);
        } else {
            return va_function_mock->invoke(args..., va_args);
        }
    }
};

}  // namespace mcga::test
