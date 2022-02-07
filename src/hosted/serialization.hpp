#pragma once

#include <mcga/proc.hpp>

#include "core/warning.hpp"

namespace mcga::test {

void mcga_read(proc::binary_reader auto& reader, String& obj) {
    obj = String(proc::read_as<std::string>(reader));
}

void mcga_write(proc::binary_writer auto& writer, const String& obj) {
    proc::write_from(writer, std::string(obj.c_str()));
}

void mcga_read(proc::binary_reader auto& reader, Context& obj) {
    proc::read_into(
      reader, obj.functionName, obj.fileName, obj.line, obj.column);
}

void mcga_write(proc::binary_writer auto& writer, const Context& obj) {
    proc::write_from(
      writer, obj.functionName, obj.fileName, obj.line, obj.column);
}

void mcga_read(proc::binary_reader auto& reader, Warning::Note& obj) {
    proc::read_into(reader, obj.type, obj.message, obj.context);
}

void mcga_write(proc::binary_writer auto& writer, const Warning::Note& obj) {
    proc::write_from(writer, obj.type, obj.message, obj.context);
}

void mcga_read(proc::binary_reader auto& reader, Warning& obj) {
    proc::read_into(reader, obj.message, obj.context);
    auto numNotes = proc::read_as<std::size_t>(reader);
    for (std::size_t i = 0; i < numNotes; i++) {
        obj.notes.push_back(proc::read_as<mcga::test::Warning::Note>(reader));
    }
}

void mcga_write(proc::binary_writer auto& writer, const Warning& obj) {
    proc::write_from(writer, obj.message, obj.context, obj.notes.getSizeSlow());
    for (const auto& note: obj.notes) {
        proc::write_from(writer, note);
    }
}

}  // namespace mcga::test
