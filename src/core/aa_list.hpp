#pragma once

#include <new>
#include <type_traits>

namespace mcga::test {

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
    struct iterator
            : std::iterator<std::forward_iterator_tag, T, std::size_t, T*, T&> {
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
        if (tail == nullptr) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        return true;
    }

    bool push_front(T value) {
        const auto slot = Allocator::allocate();
        if (slot == nullptr) {
            return false;
        }
        head = new (slot) node{std::move(value), head};
        return true;
    }

    void clear() {
        while (head != nullptr) {
            auto next = head->next;
            std::destroy_at(head);
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
