#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include <core/matcher.hpp>

#include "matcher_plugin.hpp"

AddArgument(int, flagEnableMatchers)
    .ArgumentType("0|1 ")
    .Name("enable_matchers")
    .Short("m")
    .Description("Enable usage of the matcher library")
    .DefaultValue(1)
    .ImplicitValue(1);


namespace kktest {

Plugin* matcherPlugin = new MatcherPlugin("kktest");

bool MatcherPlugin::isEnabled() const {
    Matcher::testingStarted = true;
    return flagEnableMatchers != 0;
}

void MatcherPlugin::install() {
    Matcher::matcherPluginEnabled = true;
    TestingDriver::addBeforeTestHook([this](Test*) {
        testsRunningCounter += 1;
        Matcher::duringTest = true;
    });

    TestingDriver::addAfterTestHook([this](Test*) {
        testsRunningCounter -= 1;
        if (testsRunningCounter == 0) {
            Matcher::duringTest = false;
            Matcher::cleanupMatchersCreatedDuringTests();
        }
    });
}

}
