#ifndef KKTEST_CORE_SRC_HOOKS_HPP_
#define KKTEST_CORE_SRC_HOOKS_HPP_

#include <tuple>

#include "core/src/group.hpp"
#include "core/src/test.hpp"

namespace kktest {

class Hooks {
 public:
    typedef std::function<void(const Test&)> BeforeTestHook;
    typedef std::function<void(const ExecutedTest&)> AfterTestHook;
    typedef std::function<void(GroupPtr)> BeforeGroupHook;
    typedef std::function<void(GroupPtr)> AfterGroupHook;
    typedef std::function<void()> AfterInitHook;
    typedef std::function<void()> BeforeDestroyHook;
    typedef std::function<void(const std::exception&)> BeforeForceDestroyHook;

    enum Type {
        AFTER_INIT = 0,
        BEFORE_GROUP = 1,
        AFTER_GROUP = 2,
        BEFORE_TEST = 3,
        AFTER_TEST = 4,
        BEFORE_DESTROY = 5,
        BEFORE_FORCE_DESTROY = 6,
    };

    template<Type t, class H>
    void addHook(const H& hook) {
        std::get<t>(hooks).push_back(hook);
    }

    template<Type t, class... Args>
    void runHooks(const Args&... args) {
        for (const auto& hook : std::get<t>(hooks)) {
            hook(args...);
        }
    }

 private:
    std::tuple<
        std::vector<AfterInitHook>,
        std::vector<BeforeGroupHook>,
        std::vector<AfterGroupHook>,
        std::vector<BeforeTestHook>,
        std::vector<AfterTestHook>,
        std::vector<BeforeDestroyHook>,
        std::vector<BeforeForceDestroyHook>
    > hooks;
};

}

#endif
