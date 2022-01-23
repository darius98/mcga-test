#pragma once

#include <mcga/test.hpp>

#include <compare>

#include "disallow_copy_and_move.hpp"

namespace mcga::test {

struct Callback {
    Executable body;
    Callback* next;
};

class CallbackList {
    Callback* head = nullptr;
    Callback* tail = nullptr;

  public:
    class iterator {
        Callback* node;

        explicit iterator(Callback* node) noexcept: node(node) {
        }

      public:
        auto operator<=>(const iterator& rhs) const noexcept = default;

        Executable* operator->() const noexcept {
            return &node->body;
        }
        Executable& operator*() const noexcept {
            return node->body;
        }

        iterator& operator++() {
            node = node->next;
            return *this;
        }

        iterator operator++(int) {
            auto copy = *this;
            node = node->next;
            return copy;
        }

        friend class CallbackList;
    };

    CallbackList() = default;
    ~CallbackList();

    MCGA_DISALLOW_COPY_AND_MOVE(CallbackList);

    void push_back(Executable exe);
    void push_front(Executable exe);
    void clear();

    [[nodiscard]] iterator begin() const;
    [[nodiscard]] iterator end() const;
};

}  // namespace mcga::test
