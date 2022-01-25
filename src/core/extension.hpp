#pragma once

#include <array>
#include <optional>

#include "group.hpp"
#include "test.hpp"
#include "warning.hpp"

namespace mcga::test {

struct Extension {
    void* data;
    void (*init)(void*);
    void (*onGroupDiscovered)(void*, GroupPtr);
    void (*onTestDiscovered)(void*, const Test&);
    void (*onWarning)(void*, const Warning&);
    void (*beforeTestExecution)(void*,
                                const Test&,
                                std::optional<Test::ExecutionInfo>&);
    void (*afterTestExecution)(void*, const Test&);
    void (*destroy)(void*);
};

template<class T>
Extension makeExtension(T* ext) {
    return {
      .data = ext,
      .init =
        [](void* raw) {
            if constexpr (requires(T * t) { {t->init()}; }) {
                static_cast<T*>(raw)->init();
            }
        },
      .onGroupDiscovered =
        [](void* raw, GroupPtr group) {
            if constexpr (requires(T * t) { {t->onGroupDiscovered(group)}; }) {
                static_cast<T*>(raw)->onGroupDiscovered(group);
            }
        },
      .onTestDiscovered =
        [](void* raw, const Test& test) {
            if constexpr (requires(T * t) { {t->onTestDiscovered(test)}; }) {
                static_cast<T*>(raw)->onTestDiscovered(test);
            }
        },
      .onWarning =
        [](void* raw, const Warning& warn) {
            if constexpr (requires(T * t) { {t->onWarning(warn)}; }) {
                static_cast<T*>(raw)->onWarning(warn);
            }
        },
      .beforeTestExecution =
        [](void* raw,
           const Test& test,
           std::optional<Test::ExecutionInfo>& execution) {
            if constexpr (requires(T * t) {
                              {t->beforeTestExecution(test, execution)};
                          }) {
                static_cast<T*>(raw)->beforeTestExecution(test, execution);
            } else if constexpr (requires(T * t) {
                                     {t->beforeTestExecution(test)};
                                 }) {
                static_cast<T*>(raw)->beforeTestExecution(test);
            }
        },
      .afterTestExecution =
        [](void* raw, const Test& test) {
            if constexpr (requires(T * t) { {t->afterTestExecution(test)}; }) {
                static_cast<T*>(raw)->afterTestExecution(test);
            }
        },
      .destroy =
        [](void* raw) {
            if constexpr (requires(T * t) { {t->destroy()}; }) {
                static_cast<T*>(raw)->destroy();
            }
        },
    };
}

template<class... T>
auto makeExtensionArray(T*... ext) {
    return std::array<Extension, sizeof...(T)>{makeExtension(ext)...};
}

}  // namespace mcga::test
