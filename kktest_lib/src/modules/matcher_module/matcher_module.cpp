#include <core/driver.hpp>
#include <core/matcher.hpp>
#include <core/test.hpp>
#include "matcher_module.hpp"


namespace kktest {

bool MatcherModule::isEnabled() const {
    return true;
}

void MatcherModule::install() {
    TestingDriver::addAfterTestHook([](Test*) {
        Matcher::cleanupMatchersCreatedDuringTests();
    });
}

}
