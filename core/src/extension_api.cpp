#include "core/src/extension_api.hpp"

using namespace cppli;

namespace kktest {

void ExtensionApi::beforeTest(const Hooks::BeforeTest& hook) {
    hooks.addHook<Hooks::BEFORE_TEST>(hook);
}

void ExtensionApi::afterTest(const Hooks::AfterTest& hook) {
    hooks.addHook<Hooks::AFTER_TEST>(hook);
}

void ExtensionApi::beforeGroup(const Hooks::BeforeGroup& hook) {
    hooks.addHook<Hooks::BEFORE_GROUP>(hook);
}

void ExtensionApi::afterGroup(const Hooks::AfterGroup& hook) {
    hooks.addHook<Hooks::AFTER_GROUP>(hook);
}

void ExtensionApi::afterInit(const Hooks::AfterInit& hook) {
    hooks.addHook<Hooks::AFTER_INIT>(hook);
}

void ExtensionApi::beforeDestroy(const Hooks::BeforeDestroy& hook) {
    hooks.addHook<Hooks::BEFORE_DESTROY>(hook);
}

void ExtensionApi::beforeForceDestroy(const Hooks::BeforeForceDestroy& hook) {
    hooks.addHook<Hooks::BEFORE_FORCE_DESTROY>(hook);
}

const Hooks& ExtensionApi::getHooks() const {
    return hooks;
}

void Extension::registerCommandLineArgs(Parser&) {}

void Extension::init(ExtensionApi*) {}

void Extension::destroy() {}

}
