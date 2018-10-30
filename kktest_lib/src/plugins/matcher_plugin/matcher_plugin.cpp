#include <core/driver.hpp>
#include <core/matcher.hpp>
#include "matcher_plugin.hpp"


namespace kktest {

void MatcherPlugin::install() {
    TestingDriver::addAfterTestHook([](Test*) {
        Matcher::cleanupMatchersCreatedDuringTests();
    });
}

}
