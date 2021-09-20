#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "mcga/proc.hpp"
#include "mcga/test.hpp"

namespace mcga::test {

struct Warning {
    struct Note {
        std::string message;
        std::optional<Context> context;

        Note() = default;
        Note(std::string message, std::optional<Context> context);
    };

    std::string message;
    std::optional<Context> context;
    std::vector<Note> notes;

    Warning() = default;
    explicit Warning(std::string message,
                     std::optional<Context> context = std::nullopt,
                     std::vector<Note> notes = std::vector<Note>());

    Warning& addNote(std::string message, std::optional<Context> context);
};

}  // namespace mcga::test

namespace mcga::proc {

template<>
inline Message& Message::operator>>(mcga::test::Context& obj) {
    (*this) >> obj.verb >> obj.functionName >> obj.fileName >> obj.line
      >> obj.column;
    return *this;
}

template<>
inline Message::BytesConsumer&
  Message::BytesConsumer::add(const mcga::test::Context& obj) {
    add(obj.verb, obj.functionName, obj.fileName, obj.line, obj.column);
    return *this;
}

template<>
inline Message& Message::operator>>(mcga::test::Warning& obj) {
    (*this) >> obj.message >> obj.context >> obj.notes;
    return *this;
}

template<>
inline Message::BytesConsumer&
  Message::BytesConsumer::add(const mcga::test::Warning& obj) {
    add(obj.message, obj.context, obj.notes);
    return *this;
}

template<>
inline Message& Message::operator>>(mcga::test::Warning::Note& obj) {
    (*this) >> obj.message >> obj.context;
    return *this;
}

template<>
inline Message::BytesConsumer&
  Message::BytesConsumer::add(const mcga::test::Warning::Note& obj) {
    add(obj.message, obj.context);
    return *this;
}

}  // namespace mcga::proc
