#include <core/driver.hpp>
#include <core/matcher.hpp>

using namespace std;


namespace kktest {

string Description::toString() const {
    return stream.str();
}

void* Matcher::operator new(size_t size) noexcept {
    void* p = malloc(size);
    if (testingStarted) {
        if (duringTest) {
            matchersAllocatedDuringTests.insert(p);
        } else {
            if (!matcherPluginEnabled) {
                cout << "Unable to use matchers, matcher plugin is not enabled.\n";
            } else {
                cout << "Matcher instantiated outside tests!\n";
            }
            exit(0);
        }
    }
    return p;
}

void Matcher::operator delete(void* obj) noexcept {
    matchersAllocatedDuringTests.erase(obj);
    free(obj);
}

void Matcher::cleanupMatchersCreatedDuringTests() {
    for (void* obj: matchersAllocatedDuringTests) {
        free(obj);
    }
    matchersAllocatedDuringTests.clear();
}

set<void*> Matcher::matchersAllocatedDuringTests;
bool Matcher::duringTest = false;
bool Matcher::testingStarted = false;
bool Matcher::matcherPluginEnabled = false;

}
