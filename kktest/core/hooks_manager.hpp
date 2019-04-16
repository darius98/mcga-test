#pragma once

#include <stdexcept>

#include "kktest/core/executed_test.hpp"
#include "kktest/core/warning.hpp"

namespace kktest {

class HooksManager {
  public:
    typedef std::function<void(const Test&)> BeforeTest;
    typedef std::function<void(const ExecutedTest&)> AfterTest;
    typedef std::function<void(GroupPtr)> BeforeGroup;
    typedef std::function<void(GroupPtr)> AfterGroup;
    typedef std::function<void()> AfterInit;
    typedef std::function<void()> BeforeDestroy;
    typedef std::function<void(const Warning&)> OnWarning;

    enum Type {
        AFTER_INIT = 0,
        BEFORE_GROUP = 1,
        AFTER_GROUP = 2,
        BEFORE_TEST = 3,
        AFTER_TEST = 4,
        BEFORE_DESTROY = 5,
        ON_WARNING = 6,
    };

    virtual ~HooksManager() = default;

    template<Type t, class H>
    void addHook(const H& hook) {
        std::get<t>(hooks).push_back(hook);
    }

  protected:
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

}  // namespace kktest
