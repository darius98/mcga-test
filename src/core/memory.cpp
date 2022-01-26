#include "memory.hpp"

#include <cstdlib>

#include "config.hpp"
#include "group.hpp"
#include "warning.hpp"

namespace mcga::test {

struct freelist_node {
    freelist_node* next;
};

template<int element_size, int element_align>
struct element_storage {
    static_assert(element_size >= sizeof(freelist_node));
    static_assert(element_align >= alignof(freelist_node));
    static_assert(element_align <= alignof(std::max_align_t));

    alignas(element_align) union {
        unsigned char storage[element_size];
        freelist_node node;
    };
};

template<int element_size, int element_align, int N>
struct Buffer {
    element_storage<element_size, element_align> slots[N];
    int slots_used = 0;
    freelist_node head{nullptr};

    void* allocate() {
        if (slots_used < N) {
            return slots[slots_used++].storage;
        }
        if (head.next != nullptr) {
            auto slot = head.next;
            head.next = slot->next;
            return slot;
        }
        // TODO: Have a build flag to not use malloc, fail fast instead.
        return malloc(element_size);
    }

    void deallocate(void* slot) {
        auto* node = (freelist_node*)slot;
        node->next = head.next;
        head.next = node;
    }
};

template<class T, int N>
using BufferFor = Buffer<sizeof(T), alignof(T), N>;

static BufferFor<Group, numStaticGroups> staticGroups;
static BufferFor<ExecutableList::node, numStaticCallbacks> staticExecutables;
static BufferFor<List<Warning::Note, WarningNoteAllocator>::node,
                 numStaticWarningNotes>
  staticWarningNotes;

void* GroupAllocator::allocate() {
    return staticGroups.allocate();
}

void GroupAllocator::deallocate(void* ptr) {
    staticGroups.deallocate(ptr);
}

void* ExecutableAllocator::allocate() {
    return staticExecutables.allocate();
}

void ExecutableAllocator::deallocate(void* ptr) {
    staticExecutables.deallocate(ptr);
}

void* WarningNoteAllocator::allocate() {
    return staticWarningNotes.allocate();
}

void WarningNoteAllocator::deallocate(void* ptr) {
    staticWarningNotes.deallocate(ptr);
}

}  // namespace mcga::test
