#include <core/driver.hpp>
#include <core/matcher.hpp>
#include "matcher_plugin.hpp"


namespace kktest {

void MatcherPlugin::install() {
    TestingDriver::addBeforeTestHook([](Test*) {
        Matcher::setDuringTest(true);
    });

    TestingDriver::addAfterTestHook([](Test*) {
        Matcher::setDuringTest(false);
        Matcher::cleanupMatchersCreatedDuringTests();
    });
}

}
