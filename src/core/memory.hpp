#pragma once

namespace mcga::test {

struct freelist_node {
    freelist_node* next;
};

template<int element_size, int element_align>
struct element_storage {
    static_assert(element_size >= sizeof(freelist_node));
    static_assert(element_align >= alignof(freelist_node));
    static_assert(element_align <= alignof(void*));

    union alignas(element_align) {
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
//        if (slots_used < N) {
//            return slots[slots_used++].storage;
//        }
//        if (head.next != nullptr) {
//            auto slot = head.next;
//            head.next = slot->next;
//            return slot;
//        }
#if MCGA_TEST_ALLOW_DYNAMIC_MEMORY
        return malloc(element_size);
#else
        return nullptr;
#endif
    }

    void deallocate(void* slot) {
        auto* node = static_cast<freelist_node*>(slot);
        node->next = head.next;
        head.next = node;
    }
};

template<class T, int N>
using BufferFor = Buffer<sizeof(T), alignof(T), N>;

}  // namespace mcga::test
