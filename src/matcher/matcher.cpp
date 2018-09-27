#include "matcher.hpp"
#include "../testing/testing.hpp"

using namespace runtime_testing;
using namespace std;


namespace matcher {

void* Matcher::operator new(size_t size) noexcept {
    void* p = malloc(size);
    if (isDuringTest()) {
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