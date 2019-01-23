#ifndef KKTEST_COMMON_INTERPROC_IMPL_PIPE_HPP_
#define KKTEST_COMMON_INTERPROC_IMPL_PIPE_HPP_

#include <utility>

#include <kktest_common/interproc_impl/message.hpp>

namespace kktest {
namespace interproc {

class PipeReader {
 public:
    virtual ~PipeReader() = default;

    virtual Message getNextMessage(int maxConsecutiveFailedReadAttempts) = 0;

    virtual void close() = 0;

    inline Message getNextMessage() {
        return getNextMessage(32);
    }
};

class PipeWriter {
 public:
    virtual ~PipeWriter() = default;

    inline void sendMessage(const Message& message) {
        sendBytes(message.raw(), message.getSize());
    }

    virtual void close() = 0;

    template<class... Args>
    void sendMessage(const Args... args) {
        sendMessage(Message::build(args...));
    }

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
