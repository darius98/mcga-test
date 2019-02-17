#include "core/src/extension_api.hpp"

using namespace cppli;

namespace kktest {

void ExtensionApi::beforeTest(const Hooks::TestHook& hook) {
    hooks.addHook<Hooks::BEFORE_TEST>(hook);
}

void ExtensionApi::afterTest(const Hooks::TestHook& hook) {
    hooks.addHook<Hooks::AFTER_TEST>(hook);
}

void ExtensionApi::beforeGroup(const Hooks::GroupHook& hook) {
    hooks.addHook<Hooks::BEFORE_GROUP>(hook);
}

void ExtensionApi::afterGroup(const Hooks::GroupHook& hook) {
    hooks.addHook<Hooks::AFTER_GROUP>(hook);
}

void ExtensionApi::afterInit(const Hooks::AfterInitHook& hook) {
    hooks.addHook<Hooks::AFTER_INIT>(hook);
}

void ExtensionApi::beforeDestroy(const Hooks::BeforeDestroyHook& hook) {
    hooks.addHook<Hooks::BEFORE_DESTROY>(hook);
}

void ExtensionApi::beforeForceDestroy(
        const Hooks::BeforeForceDestroyHook& hook) {
    hooks.addHook<Hooks::BEFORE_FORCE_DESTROY>(hook);
}

const Hooks& ExtensionApi::getHooks() const {
    return hooks;
}

Extension::~Extension() = default;

void Extension::registerCommandLineArgs(Parser&) {}

void Extension::init(ExtensionApi*) {}

void Extension::destroy() {}

}
