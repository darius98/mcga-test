#pragma once

#include <cstdint>

#include <stdexcept>

#include "test.hpp"
#include "warning.hpp"

namespace mcga::test {

class HooksManager {
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

    HooksManager() = default;
    HooksManager(const HooksManager&) = default;
    HooksManager(HooksManager&&) = default;

    HooksManager& operator=(const HooksManager&) = default;
    HooksManager& operator=(HooksManager&&) = default;

    virtual ~HooksManager() = default;

    template<Type t, class H>
    void addHook(const H& hook) {
        std::get<t>(hooks).push_back(hook);
    }

    template<Type t, class... Args>
    void runHooks(const Args&... args) {
        for (const auto& hook: std::get<t>(hooks)) {
            hook(args...);
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
