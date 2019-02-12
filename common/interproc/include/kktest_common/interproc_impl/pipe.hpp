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

    inline Message getNextMessage() {
        return getNextMessage(-1);
    }

    virtual std::vector<std::uint8_t> getBytes(
            int maxConsecutiveFailedReadAttempts) = 0;
};

class PipeWriter {
 public:
    virtual ~PipeWriter() = default;

    inline void sendMessage(const Message& message) {
        sendBytes(message.payload, message.getSize());
    }

    template<class... Args>
    void sendMessage(const Args... args) {
        sendMessage(Message::build(args...));
    }

    virtual void redirectStdout() = 0;

 private:
    virtual void sendBytes(void* bytes, std::size_t numBytes) = 0;
};

std::pair<PipeReader*, PipeWriter*> createAnonymousPipe();

void createNamedPipe(const String& pipeName);

void destroyNamedPipe(const String& pipeName);

PipeReader* openNamedPipeForReading(const String& pipeName);

PipeWriter* openNamedPipeForWriting(const String& pipeName);

}
}

#endif
