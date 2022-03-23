#pragma once

#include <mcga/proc/serialization.hpp>

#include "warning.hpp"

namespace mcga::test {

void write_custom(proc::binary_writer auto& writer, const String& obj) {
    const std::size_t len = std::strlen(obj.c_str());
    proc::write_from(writer, len);
    writer(obj.c_str(), len);
}

void write_custom(proc::binary_writer auto& writer, const Context& obj) {
    proc::write_from(
      writer, obj.functionName, obj.fileName, obj.line, obj.column);
}

void write_custom(proc::binary_writer auto& writer, const Warning::Note& obj) {
    proc::write_from(writer, obj.type, obj.message, obj.context);
}

void write_custom(proc::binary_writer auto& writer, const Warning& obj) {
    proc::write_from(writer, obj.message, obj.context, obj.notes.getSizeSlow());
    for (const auto& note: obj.notes) {
        proc::write_from(writer, note);
    }
}

void write_custom(proc::binary_reader auto& writer,
                  const Test::ExecutionInfo& info) {
    proc::write_from(writer,
                     info.status,
                     info.verb,
                     info.message,
                     info.context,
                     info.timeTicks);
}

// Reading these data structures requires dynamic memory allocation.
#if MCGA_TEST_ALLOW_DYNAMIC_MEMORY
void read_custom(proc::binary_reader auto& reader, String& obj) {
    obj = String(proc::read_as<std::string>(reader));
}

void read_custom(proc::binary_reader auto& reader, Context& obj) {
    proc::read_into(
      reader, obj.functionName, obj.fileName, obj.line, obj.column);
}

void read_custom(proc::binary_reader auto& reader, Warning::Note& obj) {
    proc::read_into(reader, obj.type, obj.message, obj.context);
}

void read_custom(proc::binary_reader auto& reader, Warning& obj) {
    proc::read_into(reader, obj.message, obj.context);
    auto numNotes = proc::read_as<std::size_t>(reader);
    for (std::size_t i = 0; i < numNotes; i++) {
        obj.notes.push_back(proc::read_as<mcga::test::Warning::Note>(reader));
    }
}

void read_custom(proc::binary_reader auto& reader, Test::ExecutionInfo& info) {
    proc::read_into(reader,
                    info.status,
                    info.verb,
                    info.message,
                    info.context,
                    info.timeTicks);
}
#endif

}  // namespace mcga::test
