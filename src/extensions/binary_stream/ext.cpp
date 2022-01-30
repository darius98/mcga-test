#include "ext.hpp"

#include "hosted/serialization.hpp"

namespace mcga::test {

void SocketStreamingExtension::registerCommandLineArgs(cli::Parser* parser) {
    socketPathArgument = parser->add_argument(
      cli::ArgumentSpec("stream-to-socket")
        .set_help_group("Feedback")
        .set_description("A UNIX socket with write access for piping the test "
                         "results as they become available.")
        .set_default_value(""));
}

void SocketStreamingExtension::init() {
    if (socketPathArgument->appeared()) {
        socketWriter
          = proc::createLocalClientSocket(socketPathArgument->get_value());
    }
}

void SocketStreamingExtension::onGroupDiscovered(GroupPtr group) {
    if (socketWriter != nullptr) {
        socketWriter->sendMessage(PipeMessageType::GROUP_DISCOVERED,
                                  group->getParentGroup()->getId(),
                                  group->getId(),
                                  group->getDescription(),
                                  group->isOptional());
    }
}

void SocketStreamingExtension::onTestDiscovered(const Test& test) {
    if (socketWriter != nullptr) {
        socketWriter->sendMessage(PipeMessageType::TEST_DISCOVERED,
                                  test.getId(),
                                  test.getGroup()->getId(),
                                  test.getDescription(),
                                  test.isOptional(),
                                  test.getNumAttempts(),
                                  test.getNumRequiredPassedAttempts());
    }
}

void SocketStreamingExtension::beforeTestExecution(const Test& test) {
    if (socketWriter != nullptr) {
        socketWriter->sendMessage(PipeMessageType::TEST_EXECUTION_START,
                                  test.getId());
    }
}

void SocketStreamingExtension::afterTestExecution(const Test& test) {
    if (socketWriter != nullptr) {
        const Test::ExecutionInfo& lastExecution = test.getLastExecution();
        socketWriter->sendMessage(PipeMessageType::TEST_EXECUTION_FINISH,
                                  test.getId(),
                                  lastExecution.status,
                                  lastExecution.timeTicks,
                                  lastExecution.message,
                                  lastExecution.context);
    }
}

void SocketStreamingExtension::onWarning(const Warning& warning) {
    if (socketWriter != nullptr) {
        socketWriter->sendMessage(PipeMessageType::WARNING, warning);
    }
}

void SocketStreamingExtension::destroy() {
    if (socketWriter != nullptr) {
        socketWriter->sendMessage(PipeMessageType::DONE);
    }
}

}  // namespace mcga::test
