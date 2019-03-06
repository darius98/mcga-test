#pragma once

#include <string>
#include <vector>

#include "common/interproc/src/message.hpp"

namespace kktest::interproc {

class PipeReader {
 public:
    virtual ~PipeReader() = default;

    // If maxConsecutiveFailedReadAttempts = -1, we block instead.
    virtual Message getNextMessage(int maxConsecutiveFailedReadAttempts) = 0;

    Message getNextMessage();

    virtual std::vector<std::uint8_t> getBytes(
            int maxConsecutiveFailedReadAttempts) = 0;

    std::vector<std::uint8_t> getBytes();

 protected:
    static std::size_t GetMessageSize(const Message& message);
};

class PipeWriter {
 public:
    virtual ~PipeWriter() = default;

    void sendMessage(const Message& message);

    template<class... Args>
    void sendMessage(const Args... args) {
        sendMessage(Message::Build(args...));
    }

 private:
    virtual void sendBytes(std::uint8_t* bytes, std::size_t numBytes) = 0;
};

void redirectStdoutToPipe(PipeWriter* writer);

std::pair<PipeReader*, PipeWriter*> createAnonymousPipe();

void createNamedPipe(const std::string& pipeName);

void destroyNamedPipe(const std::string& pipeName);

PipeReader* openNamedPipeForReading(const std::string& pipeName);

PipeWriter* openNamedPipeForWriting(const std::string& pipeName);

}
