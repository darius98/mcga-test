#pragma once

#include <cstdint>

#include <stdexcept>

#include "test.hpp"
#include "warning.hpp"

namespace mcga::test {

class ExtensionApi {
  public:
    using OnGroupDiscovered = std::function<void(GroupPtr)>;
    using OnTestDiscovered = std::function<void(const Test&)>;
    using OnWarning = std::function<void(const Warning&)>;
    using OnTestExecutionStart = std::function<void(const Test&)>;
    using OnTestExecutionFinish = std::function<void(const Test&)>;
    using BeforeDestroy = std::function<void()>;

    enum Type : std::uint8_t {
        ON_GROUP_DISCOVERED = 0,
        ON_TEST_DISCOVERED = 1,
        ON_WARNING = 2,
        ON_TEST_EXECUTION_START = 3,
        ON_TEST_EXECUTION_FINISH = 4,
        BEFORE_DESTROY = 5,
    };

    ExtensionApi() = default;
    ExtensionApi(const ExtensionApi&) = default;
    ExtensionApi(ExtensionApi&&) = default;

    ExtensionApi& operator=(const ExtensionApi&) = default;
    ExtensionApi& operator=(ExtensionApi&&) = default;

    virtual ~ExtensionApi() = default;

    template<Type t, class H>
    void addHook(H&& hook) {
        std::get<t>(hooks).push_back(std::forward<H>(hook));
    }

    template<Type t, class... Args>
    void runHooks(Args&&... args) {
        for (const auto& hook: std::get<t>(hooks)) {
            hook(std::forward<Args>(args)...);
        }
    }

  private:
    std::tuple<std::vector<OnGroupDiscovered>,
               std::vector<OnTestDiscovered>,
               std::vector<OnWarning>,
               std::vector<OnTestExecutionStart>,
               std::vector<OnTestExecutionFinish>,
               std::vector<BeforeDestroy>>
      hooks;
};

}  // namespace mcga::test
