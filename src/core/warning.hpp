#pragma once

#include <optional>
#include <utility>

#include <mcga/test.hpp>

#include "aa_list.hpp"
#include "memory.hpp"

namespace mcga::test {

enum class WarningNoteType : std::uint8_t {
    TEST = 1,
    GROUP = 2,
    SET_UP = 3,
    TEAR_DOWN = 4,
    CLEANUP = 5,
};

struct WarningNoteAllocator {
    [[nodiscard]] static void* allocate();
    static void deallocate(void* ptr);
};

struct Warning {
    struct Note {
        WarningNoteType type;
        String message;
        std::optional<Context> context;

        Note() = default;
        Note(WarningNoteType type,
             String message,
             std::optional<Context> context);
    };

    String message;
    std::optional<Context> context;
    List<Warning::Note, WarningNoteAllocator> notes;

    Warning() = default;
    explicit Warning(String message, std::optional<Context> context = {});

    Warning& addNote(WarningNoteType type,
                     String message,
                     std::optional<Context> context = {}) &;
    Warning&& addNote(WarningNoteType type,
                      String message,
                      std::optional<Context> context = {}) &&;
};

}  // namespace mcga::test
