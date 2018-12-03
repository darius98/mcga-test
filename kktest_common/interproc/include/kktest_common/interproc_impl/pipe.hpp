#ifndef KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_PIPE_HPP_
#define KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_PIPE_HPP_

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

    virtual void sendMessage(const Message& message) = 0;

    virtual void close() = 0;
};

std::pair<PipeReader*, PipeWriter*> createAnonymousPipe();

void createNamedPipe(const char* pipeName);

PipeReader* openNamedPipeForReading(const char* pipeName);

PipeWriter* openNamedPipeForWriting(const char* pipeName);

}  // namespace interproc
}  // namespace kktest

#endif  // KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_PIPE_HPP_
