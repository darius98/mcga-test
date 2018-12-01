#include "extension_api_impl.hpp"

namespace kktest {

Extension::~Extension() = default;

void Extension::registerCommandLineArguments(ArgumentsApi*) {}

void Extension::init(ExtensionApi*) {}

void Extension::destroy() {}

void ExtensionApiImpl::addBeforeTestHook(const TestHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_TEST>(hook);
}

void ExtensionApiImpl::addAfterTestHook(const TestHook& hook) {
    hooks.addHook<TestingDriverHooks::AFTER_TEST>(hook);
}

void ExtensionApiImpl::addBeforeGroupHook(const GroupHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_GROUP>(hook);
}

void ExtensionApiImpl::addAfterGroupHook(const GroupHook& hook) {
    hooks.addHook<TestingDriverHooks::AFTER_GROUP>(hook);
}

void ExtensionApiImpl::addAfterInitHook(const AfterInitHook& hook) {
    hooks.addHook<TestingDriverHooks::AFTER_INIT>(hook);
}

void ExtensionApiImpl::addBeforeDestroyHook(const BeforeDestroyHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_DESTROY>(hook);
}

void ExtensionApiImpl::addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook) {
    hooks.addHook<TestingDriverHooks::BEFORE_FORCE_DESTROY>(hook);
}

const TestingDriverHooks& ExtensionApiImpl::getHooks() const {
    return hooks;
}

}
