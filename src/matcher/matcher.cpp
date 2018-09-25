#include "matcher.hpp"

namespace matcher {

void* BaseMatcher::operator new(std::size_t size) noexcept {
    void* p = malloc(size);
    if (runtime_testing::isDuringTest()) {
        BaseMatcher::matchersAllocatedDuringTests.insert(p);
    }
    return p;
}

void BaseMatcher::operator delete(void* obj) noexcept {
    BaseMatcher::matchersAllocatedDuringTests.erase(obj);
    free(obj);
}

void BaseMatcher::cleanup() {
    for (void* obj: BaseMatcher::matchersAllocatedDuringTests) {
        free(obj);
    }
    BaseMatcher::matchersAllocatedDuringTests.clear();
}

std::set<void*> BaseMatcher::matchersAllocatedDuringTests;

}