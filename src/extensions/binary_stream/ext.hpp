#pragma once

#include <memory>

#include "mcga/cli.hpp"
#include "mcga/proc.hpp"

#include "core/extension.hpp"

namespace mcga::test {

enum class PipeMessageType : uint8_t {
    GROUP_DISCOVERED = 0,
    TEST_DISCOVERED = 1,
    TEST_EXECUTION_START = 2,
    TEST_EXECUTION_FINISH = 3,
    DONE = 4,
    WARNING = 5,
};

class SocketStreamingExtension {
  public:
    void registerCommandLineArgs(cli::Parser* parser);

    void init();

    void onGroupDiscovered(GroupPtr group);

    void onTestDiscovered(const Test& test);

    void beforeTestExecution(const Test& test);

    void afterTestExecution(const Test& test);

    void onWarning(const Warning& warning);

    void destroy();

  private:
    std::unique_ptr<proc::PipeWriter> socketWriter = nullptr;

    cli::Argument socketPathArgument;
};

}  // namespace mcga::test
