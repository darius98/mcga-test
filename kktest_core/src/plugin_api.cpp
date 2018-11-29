#include <kktest_impl/plugin_api.hpp>
#include <iostream>
#include "driver.hpp"

using namespace std;

namespace kktest {

void addBeforeTestHook(const TestHook& hook) {
    TestingDriver::getInstance()->addHook<TestingDriverHooks::BEFORE_TEST>(hook);
}

void addAfterTestHook(const TestHook& hook) {
    TestingDriver::getInstance()->addHook<TestingDriverHooks::AFTER_TEST>(hook);
}

void addBeforeGroupHook(const GroupHook& hook) {
    TestingDriver::getInstance()->addHook<TestingDriverHooks::BEFORE_GROUP>(hook);
}

void addAfterGroupHook(const GroupHook& hook) {
    TestingDriver::getInstance()->addHook<TestingDriverHooks::AFTER_GROUP>(hook);
}

void addAfterInitHook(const AfterInitHook& hook) {
    TestingDriver::getInstance()->addHook<TestingDriverHooks::AFTER_INIT>(hook);
}

void addBeforeDestroyHook(const BeforeDestroyHook& hook) {
    TestingDriver::getInstance()->addHook<TestingDriverHooks::BEFORE_DESTROY>(hook);
}

void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook) {
    TestingDriver::getInstance()->addHook<TestingDriverHooks::BEFORE_FORCE_DESTROY>(hook);
}

Plugin::~Plugin() = default;

void Plugin::install() {}

void Plugin::uninstall() {}

ExtensionManager::~ExtensionManager() = default;

void ExtensionManager::init(const function<void(Plugin*)>&) {}

void ExtensionManager::destroy() {}

}
