#pragma once

#include <cstdint>

#include <optional>
#include <stdexcept>

#include "test.hpp"
#include "warning.hpp"

namespace mcga::test {

class ExtensionApi {
  public:
    using GlobalHook = std::function<void()>;
    using TestHook = std::function<void(const Test&)>;
    using TestExecutionHook
      = std::function<void(const Test&, std::optional<Test::ExecutionInfo>&)>;
    using GroupHook = std::function<void(GroupPtr)>;
    using WarningHook = std::function<void(const Warning&)>;

    enum Type : std::uint8_t {
        ON_GROUP_DISCOVERED = 0,
        ON_TEST_DISCOVERED = 1,
        ON_WARNING = 2,
        BEFORE_TEST_EXECUTION = 3,
        AFTER_TEST_EXECUTION = 4,
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

  private:
    template<Type t, class... Args>
    void runHooks(Args&&... args) {
        for (const auto& hook: std::get<t>(hooks)) {
            hook(std::forward<Args>(args)...);
        }
    }

    std::tuple<std::vector<GroupHook>,
               std::vector<TestHook>,
               std::vector<WarningHook>,
               std::vector<TestExecutionHook>,
               std::vector<TestHook>,
               std::vector<GlobalHook>>
      hooks;

    friend class Executor;
    friend class BoxExecutor;
    friend class ScanExecutor;
};

}  // namespace mcga::test
