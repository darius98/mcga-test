#pragma once

#include <iterator>
#include <new>
#include <type_traits>
#include <utility>

namespace mcga::test {

template<class Node>
void list_push_back(Node*& head, Node*& tail, Node* elem) {
    if (tail == nullptr) {
        head = tail = elem;
    } else {
        tail->next = elem;
        tail = elem;
    }
}

template<class Node>
void list_push_front(Node*& head, Node*& tail, Node* elem) {
    elem->next = head;
    head = elem;
}

template<class T, class Allocator>
class List {
    static_assert(std::is_nothrow_move_constructible_v<T>);

  public:
    struct node {
        T value;
        node* next;
    };

  private:
    node* head = nullptr;
    node* tail = nullptr;

  public:
    struct iterator {
        using value_type = T;
        using difference_type = std::size_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::forward_iterator_tag;

        node* at;

        bool operator==(const iterator& rhs) const noexcept {
            return at == rhs.at;
        }

        bool operator!=(const iterator& rhs) const noexcept {
            return at != rhs.at;
        }

        T* operator->() const noexcept {
            return std::addressof(at->value);
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
    };

    List() = default;
    ~List() {
        clear();
    }

    List(List&& other) noexcept
            : head(std::exchange(other.head, nullptr)),
              tail(std::exchange(other.tail, nullptr)) {
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            head = std::exchange(other.head, nullptr);
            tail = std::exchange(other.tail, nullptr);
        }
        return *this;
    }

    bool push_back(T value) {
        const auto slot = Allocator::allocate();
        if (slot == nullptr) {
            return false;
        }
        const auto n = new (slot) node{std::move(value), nullptr};
        list_push_back(head, tail, n);
        return true;
    }

    bool push_front(T value) {
        const auto slot = Allocator::allocate();
        if (slot == nullptr) {
            return false;
        }
        const auto n = new (slot) node{std::move(value), nullptr};
        list_push_front(head, tail, n);
        return true;
    }

    void clear() {
        while (head != nullptr) {
            auto next = head->next;
            head->~node();
            Allocator::deallocate(head);
            head = next;
        }
        tail = nullptr;
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
        return iterator{.at = head};
    }

    [[nodiscard]] iterator end() const {
        return iterator{.at = nullptr};
    }
};

}  // namespace mcga::test
