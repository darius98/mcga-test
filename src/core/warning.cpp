#include "warning.hpp"

namespace mcga::test {

Warning::Note::Note(WarningNoteType type,
                    String message,
                    std::optional<Context> context)
        : type(type), message(std::move(message)), context(context) {
}

Warning::Warning(String message, std::optional<Context> context)
        : message(std::move(message)), context(context) {
}

Warning& Warning::addNote(WarningNoteType type,
                          String noteMessage,
                          std::optional<Context> noteContext) & {
    // Even if this push back fails to allocate (in the no-dynamic-memory mode),
    // nothing changes. We usually emit a warning, but in this case, it would be
    // weird to emit a warning that the warnings are too big and don't have
    // enough memory.
    notes.push_back(Note{type, std::move(noteMessage), noteContext});
    return *this;
}

Warning&& Warning::addNote(WarningNoteType type,
                           String noteMessage,
                           std::optional<Context> noteContext) && {
    // Even if this push back fails to allocate (in the no-dynamic-memory mode),
    // nothing changes. We usually emit a warning, but in this case, it would be
    // weird to emit a warning that the warnings are too big and don't have
    // enough memory.
    notes.push_back(Note{type, std::move(noteMessage), noteContext});
    return std::move(*this);
}

}  // namespace mcga::test
