#include "driver.hpp"
#include "matcher.hpp"

using namespace kktest;
using namespace std;


namespace kktest {

void* Matcher::operator new(size_t size) noexcept {
    void* p = malloc(size);
    if (TestingDriver::isDuringTest()) {
        matchersAllocatedDuringTests.insert(p);
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

}
