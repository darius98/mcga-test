#include "null_stream.hpp"

namespace runtime_testing {
namespace utils {

int NullStreamBuffer::overflow(int c) {
    this->setp(this->buffer, this->buffer + 64);
    return (c == EOF) ? '\0' : c;
}

NullStream::NullStream() : std::ostream(this) {}

NullStream nullStream;

}
}
