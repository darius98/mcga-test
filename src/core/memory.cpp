#include "memory.hpp"

#include <cstdlib>

#include "config.hpp"
#include "group.hpp"

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
static BufferFor<Callback, numStaticCallbacks> staticCallbacks;

void* allocate_group() {
    return staticGroups.allocate();
}

void deallocate_group(void* group) {
    staticGroups.deallocate(group);
}

void* allocate_callback() {
    return staticCallbacks.allocate();
}

void deallocate_callback(void* callback) {
    staticCallbacks.deallocate(callback);
}

}  // namespace mcga::test
