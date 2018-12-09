#ifndef KKTEST_CORE_SRC_EXTENSION_API_IMPL_HPP_
#define KKTEST_CORE_SRC_EXTENSION_API_IMPL_HPP_

#include <tuple>
#include <vector>

#include <kktest_impl/extension_api.hpp>

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
        std::vector<AfterInitHook>,
        std::vector<GroupHook>,
        std::vector<GroupHook>,
        std::vector<TestHook>,
        std::vector<TestHook>,
        std::vector<BeforeDestroyHook>,
        std::vector<BeforeForceDestroyHook>
    > hooks;
};

class ExtensionApiImpl: public ExtensionApi {
 public:
    void beforeTest(const TestHook& hook) override;
    void afterTest(const TestHook& hook) override;
    void beforeGroup(const GroupHook& hook) override;
    void afterGroup(const GroupHook& hook) override;
    void afterInit(const AfterInitHook& hook) override;
    void beforeDestroy(const BeforeDestroyHook& hook) override;
    void beforeForceDestroy(const BeforeForceDestroyHook& hook) override;

    const Hooks& getHooks() const;

 private:
    Hooks hooks;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_EXTENSION_API_IMPL_HPP_
