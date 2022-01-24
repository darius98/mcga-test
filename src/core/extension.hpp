#pragma once

#include <array>

#include "extension_api.hpp"

namespace mcga::test {

struct Extension {
    void* data;
    void (*init)(void*, ExtensionApi* api);
    void (*destroy)(void*);
};

template<class T>
Extension makeExtension(T* ext) {
    return {
      .data = ext,
      .init =
        [](void* raw, ExtensionApi* api) {
            static_cast<T*>(raw)->init(api);
        },
      .destroy =
        [](void* raw) {
            static_cast<T*>(raw)->destroy();
        },
    };
}

template<class... T>
auto makeExtensionArray(T*... ext) {
    return std::array<Extension, sizeof...(T)>{makeExtension(ext)...};
}

}  // namespace mcga::test
