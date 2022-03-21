#pragma once

#include "mcga/proc/serialization.hpp"

#include "core/extension.hpp"
#include "core/serialization.hpp"

namespace mcga::test {

enum class PipeMessageType : uint8_t {
    GROUP_DISCOVERED = 0,
    TEST_DISCOVERED = 1,
    TEST_EXECUTION_START = 2,
    TEST_EXECUTION_FINISH = 3,
    DONE = 4,
    WARNING = 5,
};

template<mcga::proc::binary_writer Writer>
class BinaryStreamExtension {
    Writer writer;
    void (*flush_cb)(Writer&);

    void flush() {
        if (flush_cb) {
            flush_cb(writer);
        }
    }

  public:
    explicit BinaryStreamExtension(Writer writer,
                                   void (*flush_cb)(Writer&) = nullptr)
            : writer(std::move(writer)), flush_cb(flush_cb) {
    }

    void onGroupDiscovered(GroupPtr group) {
        proc::write_from(
          writer,
          PipeMessageType::GROUP_DISCOVERED,
          group->hasParentGroup() ? group->getParentGroup()->getId() : 0,
          group->getId(),
          group->getDescription(),
          group->isOptional());
        flush();
    }

    void onTestDiscovered(const Test& test) {
        proc::write_from(writer,
                         PipeMessageType::TEST_DISCOVERED,
                         test.getId(),
                         test.getGroup()->getId(),
                         test.getDescription(),
                         test.isOptional(),
                         test.getNumAttempts(),
                         test.getNumRequiredPassedAttempts());
        flush();
    }

    void beforeTestExecution(const Test& test) {
        proc::write_from(
          writer, PipeMessageType::TEST_EXECUTION_START, test.getId());
        flush();
    }

    void afterTestExecution(const Test& test) {
        const Test::ExecutionInfo& lastExecution = test.getLastExecution();
        proc::write_from(writer,
                         PipeMessageType::TEST_EXECUTION_FINISH,
                         test.getId(),
                         lastExecution.status,
                         lastExecution.timeTicks,
                         lastExecution.message,
                         lastExecution.context);
        flush();
    }

    void onWarning(const Warning& warning) {
        proc::write_from(writer, PipeMessageType::WARNING, warning);
        flush();
    }
};

template<mcga::proc::binary_writer Writer>
BinaryStreamExtension(Writer) -> BinaryStreamExtension<Writer>;

template<mcga::proc::binary_writer Writer, class Any>
BinaryStreamExtension(Writer, Any) -> BinaryStreamExtension<Writer>;

}  // namespace mcga::test
