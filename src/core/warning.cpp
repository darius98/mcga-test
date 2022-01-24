#include "warning.hpp"

namespace mcga::test {

Warning::Note::Note(String message, std::optional<Context> context)
        : message(std::move(message)), context(context) {
}

Warning::Warning(String message, std::optional<Context> context)
        : message(std::move(message)), context(context) {
}

Warning& Warning::addNote(String noteMessage,
                          std::optional<Context> noteContext) & {
    notes.push_back(Note{std::move(noteMessage), noteContext});
    return *this;
}

Warning&& Warning::addNote(String noteMessage,
                           std::optional<Context> noteContext) && {
    notes.push_back(Note{std::move(noteMessage), noteContext});
    return std::move(*this);
}

}  // namespace mcga::test
