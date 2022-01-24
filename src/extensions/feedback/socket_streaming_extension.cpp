#include "socket_streaming_extension.hpp"

namespace mcga::test {

void SocketStreamingExtension::registerCommandLineArgs(cli::Parser* parser) {
    socketPathArgument = parser->add_argument(
      cli::ArgumentSpec("stream-to-socket")
        .set_help_group("Feedback")
        .set_description("A UNIX socket with write access for piping the test "
                         "results as they become available.")
        .set_default_value(""));
}

void SocketStreamingExtension::init(ExtensionApi* api) {
    if (socketPathArgument->appeared()) {
        socketWriter = std::unique_ptr<proc::PipeWriter>(
          proc::createLocalClientSocket(socketPathArgument->get_value()));
        addPipeHooks(socketWriter.get(), api);
    }
}

void SocketStreamingExtension::destroy() {
    if (socketWriter != nullptr) {
        socketWriter->sendMessage(PipeMessageType::DONE);
    }
}

void SocketStreamingExtension::addPipeHooks(proc::PipeWriter* pipe,
                                     ExtensionApi* api) {
    api->addHook<ExtensionApi::ON_GROUP_DISCOVERED>([pipe](GroupPtr group) {
        pipe->sendMessage(PipeMessageType::GROUP_DISCOVERED,
                          group->getParentGroup()->getId(),
                          group->getId(),
                          group->getDescription(),
                          group->isOptional());
    });

    api->addHook<ExtensionApi::ON_TEST_DISCOVERED>([pipe](const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_DISCOVERED,
                          test.getId(),
                          test.getGroup()->getId(),
                          test.getDescription(),
                          test.isOptional(),
                          test.getNumAttempts(),
                          test.getNumRequiredPassedAttempts());
    });

    api->addHook<ExtensionApi::BEFORE_TEST_EXECUTION>(
      [pipe](const Test& test, std::optional<Test::ExecutionInfo>&) {
          pipe->sendMessage(PipeMessageType::TEST_EXECUTION_START,
                            test.getId());
      });

    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>([pipe](const Test& test) {
        const Test::ExecutionInfo& lastExecution = test.getLastExecution();
        pipe->sendMessage(PipeMessageType::TEST_EXECUTION_FINISH,
                          test.getId(),
                          lastExecution.status,
                          lastExecution.timeTicks,
                          lastExecution.message,
                          lastExecution.context);
    });

    api->addHook<ExtensionApi::ON_WARNING>([pipe](const Warning& warning) {
        pipe->sendMessage(PipeMessageType::WARNING, warning);
    });
}

}  // namespace mcga::test
