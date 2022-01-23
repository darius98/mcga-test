#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "mcga/proc.hpp"
#include "mcga/test.hpp"

namespace mcga::test {

struct Warning {
    struct Note {
        String message;
        std::optional<Context> context;

        Note() = default;
        Note(String message, std::optional<Context> context);
    };

    String message;
    std::optional<Context> context;
    std::vector<Note> notes;

    Warning() = default;
    explicit Warning(String message,
                     std::optional<Context> context = {},
                     std::vector<Note> notes = {});

    Warning& addNote(String message, std::optional<Context> context);
};

}  // namespace mcga::test

namespace mcga::proc {

template<>
inline Message& Message::operator>>(mcga::test::String& obj) {
    std::string s;
    (*this) >> s;
    obj = mcga::test::String(s);
    return *this;
}

template<>
inline Message::BytesConsumer&
  Message::BytesConsumer::add(const mcga::test::String& obj) {
    return add(std::string(obj.c_str()));
}

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
