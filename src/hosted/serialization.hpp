#pragma once

#include <mcga/proc.hpp>

#include "core/warning.hpp"

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
    (*this) >> obj.functionName >> obj.fileName >> obj.line >> obj.column;
    return *this;
}

template<>
inline Message::BytesConsumer&
  Message::BytesConsumer::add(const mcga::test::Context& obj) {
    add(obj.functionName, obj.fileName, obj.line, obj.column);
    return *this;
}

template<>
inline Message& Message::operator>>(mcga::test::Warning::Note& obj) {
    (*this) >> obj.type >> obj.message >> obj.context;
    return *this;
}

template<>
inline Message::BytesConsumer&
  Message::BytesConsumer::add(const mcga::test::Warning::Note& obj) {
    add(obj.type, obj.message, obj.context);
    return *this;
}

template<>
inline Message& Message::operator>>(mcga::test::Warning& obj) {
    (*this) >> obj.message >> obj.context;
    auto numNotes = read<std::size_t>();
    for (std::size_t i = 0; i < numNotes; i++) {
        obj.notes.push_back(read<mcga::test::Warning::Note>());
    }
    return *this;
}

template<>
inline Message::BytesConsumer&
  Message::BytesConsumer::add(const mcga::test::Warning& obj) {
    add(obj.message, obj.context);
    add(obj.notes.getSizeSlow());
    for (const auto& note: obj.notes) {
        add(note);
    }
    return *this;
}

}  // namespace mcga::proc
