#pragma once

#include <stdexcept>

#include "executed_test.hpp"
#include "warning.hpp"

namespace mcga::test {

class HooksManager {
  public:
    using BeforeTest = std::function<void(const Test&)>;
    using AfterTest = std::function<void(const ExecutedTest&)>;
    using BeforeGroup = std::function<void(GroupPtr)>;
    using AfterGroup = std::function<void(GroupPtr)>;
    using AfterInit = std::function<void()>;
    using BeforeDestroy = std::function<void()>;
    using OnWarning = std::function<void(const Warning&)>;

    enum Type {
        AFTER_INIT = 0,
        BEFORE_GROUP = 1,
        AFTER_GROUP = 2,
        BEFORE_TEST = 3,
        AFTER_TEST = 4,
        BEFORE_DESTROY = 5,
        ON_WARNING = 6,
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
    std::tuple<std::vector<AfterInit>,
               std::vector<BeforeGroup>,
               std::vector<AfterGroup>,
               std::vector<BeforeTest>,
               std::vector<AfterTest>,
               std::vector<BeforeDestroy>,
               std::vector<OnWarning>>
      hooks;
};

}  // namespace mcga::test
