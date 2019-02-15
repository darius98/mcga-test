#ifndef KKTEST_CORE_HOOKS_HPP_
#define KKTEST_CORE_HOOKS_HPP_

#include <tuple>

#include <extension_api/extension_api.hpp>

namespace kktest {

class Hooks {
 public:
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
    void runHooks(const Args... args) {
        for (const auto& hook : std::get<t>(hooks)) {
            hook(args...);
        }
    }

 private:
    std::tuple<
        std::vector<ExtensionApi::AfterInitHook>,
        std::vector<ExtensionApi::GroupHook>,
        std::vector<ExtensionApi::GroupHook>,
        std::vector<ExtensionApi::TestHook>,
        std::vector<ExtensionApi::TestHook>,
        std::vector<ExtensionApi::BeforeDestroyHook>,
        std::vector<ExtensionApi::BeforeForceDestroyHook>
    > hooks;
};

}

#endif
