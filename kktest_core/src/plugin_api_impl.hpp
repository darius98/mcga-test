#ifndef KKTEST_CORE_PLUGIN_API_IMPL_H_
#define KKTEST_CORE_PLUGIN_API_IMPL_H_

#include <kktest_impl/plugin_api.hpp>

namespace kktest {

class TestingDriverHooks {
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
        for (const auto& hook: std::get<t>(hooks)) {
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

class PluginApiImpl: public PluginApi {
public:
    void addBeforeTestHook(const TestHook& hook) override;
    void addAfterTestHook(const TestHook& hook) override;
    void addBeforeGroupHook(const GroupHook& hook) override;
    void addAfterGroupHook(const GroupHook& hook) override;
    void addAfterInitHook(const AfterInitHook& hook) override;
    void addBeforeDestroyHook(const BeforeDestroyHook& hook) override;
    void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook) override;

    const TestingDriverHooks& getHooks() const;
private:
    TestingDriverHooks hooks;
};

}

#endif
