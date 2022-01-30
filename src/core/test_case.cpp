#include "test_case.hpp"
#include "config.hpp"
#include "memory.hpp"

namespace mcga::test {

static BufferFor<TestCaseList::node, numStaticTestCases> staticTestCases;

void* TestCaseAllocator::allocate() {
    return staticTestCases.allocate();
}

void TestCaseAllocator::deallocate(void* ptr) {
    staticTestCases.deallocate(ptr);
}

}  // namespace mcga::test
