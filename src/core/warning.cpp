#include "warning.hpp"

namespace mcga::test {

Warning::Note::Note(String message, std::optional<Context> context)
        : message(std::move(message)), context(std::move(context)) {
}

Warning::Warning(String message,
                 std::optional<Context> context,
                 std::vector<Note> notes)
        : message(std::move(message)), context(std::move(context)),
          notes(std::move(notes)) {
}

Warning& Warning::addNote(String noteMessage,
                          std::optional<Context> noteContext) {
    notes.emplace_back(std::move(noteMessage), std::move(noteContext));
    return *this;
}

}  // namespace mcga::test
