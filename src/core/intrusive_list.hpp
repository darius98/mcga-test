#pragma once

#include <compare>
#include <type_traits>

namespace mcga::test {

template<class T, class Allocator>
class IntrusiveList {
    static_assert(std::is_nothrow_move_constructible_v<T>);
    static_assert(std::is_empty_v<Allocator>);

  public:
    struct node {
        T value;
        node* next;
    };

  private:
    node* head = nullptr;
    node* tail = nullptr;

  public:
    class iterator {
        node* at;

        explicit iterator(node* at) noexcept: at(at) {
        }

      public:
        auto operator<=>(const iterator& rhs) const noexcept = default;

        T* operator->() const noexcept {
            return &at->value;
        }
        T& operator*() const noexcept {
            return at->value;
        }

        iterator& operator++() {
            at = at->next;
            return *this;
        }

        iterator operator++(int) {
            auto copy = *this;
            at = at->next;
            return copy;
        }

        friend class IntrusiveList;
    };

    IntrusiveList() = default;
    ~IntrusiveList() {
        clear();
    }

    IntrusiveList(IntrusiveList&& other) noexcept
            : head(other.head), tail(other.tail) {
        other.head = other.tail = nullptr;
    }

    IntrusiveList& operator=(IntrusiveList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            other.head = other.tail = nullptr;
        }
        return *this;
    }

    void push_back(T value) {
        const auto slot = Allocator{}.allocate();
        const auto n = new (slot) node{std::move(value), nullptr};
        if (tail == nullptr) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }

    void push_front(T value) {
        const auto slot = Allocator{}.allocate();
        head = new (slot) node{std::move(value), head};
    }

    void clear() {
        auto n = head;
        while (n != nullptr) {
            auto next = n->next;
            n->~node();
            Allocator{}.deallocate(n);
            n = next;
        }
        head = tail = nullptr;
    }

    [[nodiscard]] std::size_t getSizeSlow() const {
        std::size_t size = 0;
        auto node = head;
        while (node != nullptr) {
            size++;
            node = node->next;
        }
        return size;
    }

    [[nodiscard]] iterator begin() const {
        return iterator(head);
    }

    [[nodiscard]] iterator end() const {
        return iterator(nullptr);
    }
};

}  // namespace mcga::test
