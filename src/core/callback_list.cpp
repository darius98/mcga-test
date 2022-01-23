#include "callback_list.hpp"

#include "memory.hpp"

#include <new>
#include <utility>

namespace mcga::test {

CallbackList::~CallbackList() {
    clear();
}

void CallbackList::push_back(Executable exe) {
    const auto slot = allocate_callback();
    const auto callback = new (slot) Callback{std::move(exe), nullptr};
    if (tail == nullptr) {
        head = tail = callback;
    } else {
        tail->next = callback;
        tail = callback;
    }
}

void CallbackList::push_front(Executable exe) {
    const auto slot = allocate_callback();
    head = new (slot) Callback{std::move(exe), head};
}

void CallbackList::clear() {
    auto node = head;
    while (node != nullptr) {
        auto next = node->next;
        node->~Callback();
        deallocate_callback(node);
        node = next;
    }
    head = tail = nullptr;
}

CallbackList::iterator CallbackList::begin() const {
    return iterator(head);
}

CallbackList::iterator CallbackList::end() const {
    return iterator(nullptr);
}

}  // namespace mcga::test
