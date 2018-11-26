#include <kktest_impl/plugin_api.hpp>
#include <iostream>
#include "driver.hpp"

using namespace std;

namespace kktest {

void addBeforeTestHook(const TestHook& hook) {
    TestingDriver::getInstance()->addHook(hook, TestingDriver::getInstance()->beforeTestHooks);
}

void addAfterTestHook(const TestHook& hook) {
    TestingDriver::getInstance()->addHook(hook, TestingDriver::getInstance()->afterTestHooks);
}

void addBeforeGroupHook(const GroupHook& hook) {
    TestingDriver::getInstance()->addHook(hook, TestingDriver::getInstance()->beforeGroupHooks);
}

void addAfterGroupHook(const GroupHook& hook) {
    TestingDriver::getInstance()->addHook(hook, TestingDriver::getInstance()->afterGroupHooks);
}

void addAfterInitHook(const AfterInitHook& hook) {
    TestingDriver::getInstance()->addHook(hook, TestingDriver::getInstance()->afterInitHooks);
}

void addBeforeDestroyHook(const BeforeDestroyHook& hook) {
    TestingDriver::getInstance()->addHook(hook, TestingDriver::getInstance()->beforeDestroyHooks);
}

void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook) {
    TestingDriver::getInstance()->addHook(hook, TestingDriver::getInstance()->beforeForceDestroyHooks);
}

vector<Plugin*>* Plugin::plugins = nullptr;

Plugin::Plugin() {
    if (plugins == nullptr) {
        plugins = new vector<Plugin*>();
    }
    plugins->push_back(this);
}

void Plugin::clean() {
    delete plugins;
}

}
