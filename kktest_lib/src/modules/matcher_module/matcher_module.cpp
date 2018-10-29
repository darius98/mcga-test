#include <core/driver.hpp>
#include <core/matcher.hpp>
#include "matcher_module.hpp"


namespace kktest {

void MatcherModule::install() {
    TestingDriver::addAfterTestHook([](Test*) {
        Matcher::cleanupMatchersCreatedDuringTests();
    });
}

}
