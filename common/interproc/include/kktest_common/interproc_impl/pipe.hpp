#ifndef KKTEST_COMMON_INTERPROC_IMPL_PIPE_HPP_
#define KKTEST_COMMON_INTERPROC_IMPL_PIPE_HPP_

#include <utility>
#include <vector>

#include <kktest_common/interproc_impl/message.hpp>

namespace kktest {
namespace interproc {

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
    static std::size_t getMessageSize(const Message& message);
};

class PipeWriter {
 public:
    virtual ~PipeWriter() = default;

    void sendMessage(const Message& message);

    template<class... Args>
    void sendMessage(const Args... args) {
        sendMessage(Message::build(args...));
    }

 private:
    virtual void sendBytes(void* bytes, std::size_t numBytes) = 0;
};

void redirectStdoutToPipe(PipeWriter* writer);

std::pair<PipeReader*, PipeWriter*> createAnonymousPipe();

void createNamedPipe(const String& pipeName);

void destroyNamedPipe(const String& pipeName);

PipeReader* openNamedPipeForReading(const String& pipeName);

PipeWriter* openNamedPipeForWriting(const String& pipeName);

}
}

#endif
