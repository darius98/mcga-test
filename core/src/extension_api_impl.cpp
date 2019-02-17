#include "core/src/extension_api_impl.hpp"

using namespace cppli;

namespace kktest {

Extension::~Extension() = default;

void Extension::registerCommandLineArgs(Parser&) {}

void Extension::init(ExtensionApi*) {}

void Extension::destroy() {}

void ExtensionApiImpl::beforeTest(const TestHook& hook) {
    hooks.addHook<Hooks::BEFORE_TEST>(hook);
}

void ExtensionApiImpl::afterTest(const TestHook& hook) {
    hooks.addHook<Hooks::AFTER_TEST>(hook);
}

void ExtensionApiImpl::beforeGroup(const GroupHook& hook) {
    hooks.addHook<Hooks::BEFORE_GROUP>(hook);
}

void ExtensionApiImpl::afterGroup(const GroupHook& hook) {
    hooks.addHook<Hooks::AFTER_GROUP>(hook);
}

void ExtensionApiImpl::afterInit(const AfterInitHook& hook) {
    hooks.addHook<Hooks::AFTER_INIT>(hook);
}

void ExtensionApiImpl::beforeDestroy(const BeforeDestroyHook& hook) {
    hooks.addHook<Hooks::BEFORE_DESTROY>(hook);
}

void ExtensionApiImpl::beforeForceDestroy(const BeforeForceDestroyHook& hook) {
    hooks.addHook<Hooks::BEFORE_FORCE_DESTROY>(hook);
}

const Hooks& ExtensionApiImpl::getHooks() const {
    return hooks;
}

}
