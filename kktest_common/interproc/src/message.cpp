#include <cstring>

#include <kktest_common/interproc_impl/message.hpp>

using std::string;

namespace kktest {
namespace interproc {

BytesConsumer& BytesConsumer::add(const string& obj) {
    add(obj.size());
    addBytes(obj.c_str(), obj.size());
    return *this;
}

BytesCounter& BytesCounter::addBytes(const void*, size_t numBytes) {
    bytesConsumed += numBytes;
    return *this;
}

size_t BytesCounter::getNumBytesConsumed() const {
    return bytesConsumed;
}

size_t Message::isSane(const void* ptr, size_t size) {
    if (size < sizeof(size_t)) {
        return 0;
    }
    auto expectedSize = (*static_cast<const size_t*>(ptr)) + sizeof(size_t);
    return expectedSize <= size ? expectedSize : 0;
}

Message::Message(size_t size): payload(malloc(size + sizeof(size_t))) {
    memcpy(payload, &size, sizeof(size_t));
    cursor = sizeof(size_t);
}

Message::Message(const Message& other) {
    auto size = other.getSize();
    payload = malloc(size + sizeof(size_t));
    memcpy(payload, other.payload, cursor);
    cursor = other.cursor;
}

Message::Message(Message&& other) noexcept: payload(other.payload), cursor(other.cursor) {
    other.payload = nullptr;
}

Message::Message(void* _payload): payload(_payload), cursor(0) {}

Message::~Message() {
    if (payload != nullptr) {
        free(payload);
    }
}

Message& Message::operator=(const Message& other) {
    if (payload != nullptr) {
        free(payload);
    }
    auto size = other.getSize();
    payload = malloc(size + sizeof(size_t));
    memcpy(payload, other.payload, cursor);
    cursor = other.cursor;
    return *this;
}

Message& Message::operator=(Message&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    payload = other.payload;
    cursor = other.cursor;
    other.payload = nullptr;
    return *this;
}

Message& Message::addBytes(const void* bytes, size_t numBytes) {
    memcpy(static_cast<uint8_t*>(payload) + cursor, bytes, numBytes);
    cursor += numBytes;
    return *this;
}

void* Message::getPayload() const {
    return payload;
}

size_t Message::getSize() const {
    return sizeof(size_t) + *static_cast<size_t*>(payload);
}

}  // namespace interproc
}  // namespace kktest
