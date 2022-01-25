#pragma once

#include <cstdint>

#include <optional>

#include "extension.hpp"
#include "test.hpp"
#include "warning.hpp"

namespace mcga::test {

class ExtensionApi {
    Extension* extensions;
    std::size_t numExtensions;

  public:
    enum Type : std::uint8_t {
        ON_GROUP_DISCOVERED = 0,
        ON_TEST_DISCOVERED = 1,
        ON_WARNING = 2,
        BEFORE_TEST_EXECUTION = 3,
        AFTER_TEST_EXECUTION = 4,
    };

    ExtensionApi(Extension* extensions, std::size_t numExtensions)
            : extensions(extensions), numExtensions(numExtensions) {
    }

    template<Type t, class... Args>
    void runHooks(Args&... args) {
        for (std::size_t i = 0; i < numExtensions; i++) {
            auto ext = extensions[i];

            // TODO: cleaner?

            if constexpr (t == ON_GROUP_DISCOVERED) {
                ext.onGroupDiscovered(ext.data, args...);
            }

            if constexpr (t == ON_TEST_DISCOVERED) {
                ext.onTestDiscovered(ext.data, args...);
            }

            if constexpr (t == ON_WARNING) {
                ext.onWarning(ext.data, args...);
            }

            if constexpr (t == BEFORE_TEST_EXECUTION) {
                ext.beforeTestExecution(ext.data, args...);
            }

            if constexpr (t == AFTER_TEST_EXECUTION) {
                ext.afterTestExecution(ext.data, args...);
            }
        }
    }
};

}  // namespace mcga::test
