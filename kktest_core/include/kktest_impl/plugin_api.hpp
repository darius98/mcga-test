#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_PLUGIN_API_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_PLUGIN_API_H_

#include <functional>

#include <kktest_impl/info.hpp>

namespace kktest {

typedef std::function<void(const TestInfo&)> TestHook;
typedef std::function<void(const GroupInfo&)> GroupHook;
typedef std::function<void()> AfterInitHook;
typedef std::function<void()> BeforeDestroyHook;
typedef std::function<void(const std::exception&)> BeforeForceDestroyHook;

void addBeforeTestHook(const TestHook& hook);
void addAfterTestHook(const TestHook& hook);
void addBeforeGroupHook(const GroupHook& hook);
void addAfterGroupHook(const GroupHook& hook);
void addAfterInitHook(const AfterInitHook& hook);
void addBeforeDestroyHook(const BeforeDestroyHook& hook);
void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook);

class Plugin {
public:
    virtual ~Plugin();

    virtual void install();

    virtual void uninstall();
};

class ExtensionManager {
public:
    virtual ~ExtensionManager();

    virtual void init(const std::function<void(Plugin*)>& addPlugin);

    virtual void destroy();
};

}

#endif
