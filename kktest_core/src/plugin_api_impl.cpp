#include "plugin_api_impl.hpp"

using namespace std;

namespace kktest {

Plugin::~Plugin() = default;

void Plugin::install(PluginApi*) {}

void Plugin::uninstall() {}

ExtensionManager::~ExtensionManager() = default;

void ExtensionManager::init(const function<void(Plugin*)>&) {}

void ExtensionManager::destroy() {}

void PluginApiImpl::addBeforeTestHook(const TestHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_TEST>(hook);
}

void PluginApiImpl::addAfterTestHook(const TestHook& hook) {
    hooks.addHook<TestingDriverHooks::AFTER_TEST>(hook);
}

void PluginApiImpl::addBeforeGroupHook(const GroupHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_GROUP>(hook);
}

void PluginApiImpl::addAfterGroupHook(const GroupHook& hook) {
    hooks.addHook<TestingDriverHooks::AFTER_GROUP>(hook);
}

void PluginApiImpl::addAfterInitHook(const AfterInitHook& hook) {
    hooks.addHook<TestingDriverHooks::AFTER_INIT>(hook);
}

void PluginApiImpl::addBeforeDestroyHook(const BeforeDestroyHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_DESTROY>(hook);
}

void PluginApiImpl::addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_FORCE_DESTROY>(hook);
}

const TestingDriverHooks& PluginApiImpl::getHooks() const {
    return hooks;
}

}
