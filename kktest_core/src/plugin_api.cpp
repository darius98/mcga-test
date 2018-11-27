#include <kktest_impl/plugin_api.hpp>
#include <iostream>
#include "driver.hpp"

using namespace std;

namespace kktest {

void addBeforeTestHook(const TestHook& hook) {
    TestingDriver::addHook<TestingDriverHooks::BEFORE_TEST>(hook);
}

void addAfterTestHook(const TestHook& hook) {
    TestingDriver::addHook<TestingDriverHooks::AFTER_TEST>(hook);
}

void addBeforeGroupHook(const GroupHook& hook) {
    TestingDriver::addHook<TestingDriverHooks::BEFORE_GROUP>(hook);
}

void addAfterGroupHook(const GroupHook& hook) {
    TestingDriver::addHook<TestingDriverHooks::AFTER_GROUP>(hook);
}

void addAfterInitHook(const AfterInitHook& hook) {
    TestingDriver::addHook<TestingDriverHooks::AFTER_INIT>(hook);
}

void addBeforeDestroyHook(const BeforeDestroyHook& hook) {
    TestingDriver::addHook<TestingDriverHooks::BEFORE_DESTROY>(hook);
}

void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook) {
    TestingDriver::addHook<TestingDriverHooks::BEFORE_FORCE_DESTROY>(hook);
}

Plugin::Plugin() {
    TestingDriver::addPlugin(this);
}

Plugin::~Plugin() = default;

bool Plugin::isEnabled() const {
    return true;
}

void Plugin::install() {}

void Plugin::uninstall() {}

}
