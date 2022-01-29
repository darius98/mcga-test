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
    ExtensionApi(Extension* extensions, std::size_t numExtensions);

    void init();
    void onGroupDiscovered(const GroupPtr& group);
    void onTestDiscovered(const Test& test);
    void beforeTestExecution(const Test& test,
                             std::optional<Test::ExecutionInfo>& info);
    void afterTestExecution(const Test& test);
    void onWarning(const Warning& warning);
    void destroy();

  private:
    template<class Callable>
    void forEach(Callable callable) {
        for (std::size_t i = 0; i < numExtensions; i++) {
            callable(extensions[i]);
        }
    }
};

}  // namespace mcga::test
