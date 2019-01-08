#include <cstdlib>
#include <cstring>

#include <kktest_common/interproc_impl/message.hpp>

using std::malloc;

namespace kktest {
namespace interproc {

BytesConsumer& BytesConsumer::add(const String& obj) {
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

Message Message::read(const void* src, size_t maxSize) {
    if (maxSize < sizeof(size_t)) {
        return invalid();
    }
    auto expectedSize = (*static_cast<const size_t*>(src)) + sizeof(size_t);
    if (expectedSize > maxSize) {
        return invalid();
    }
    void* messagePayload = malloc(expectedSize);
    memcpy(messagePayload, src, expectedSize);
    return Message(messagePayload);
}

Message Message::invalid() {
    return Message(nullptr);
}

Message::Message(size_t size): payload(malloc(size + sizeof(size_t))) {
    memcpy(payload, &size, sizeof(size_t));
    cursor = sizeof(size_t);
}

Message::Message(const Message& other) {
    if (other.isInvalid()) {
        payload = nullptr;
        cursor = 0;
    } else {
        auto size = other.getSize();
        payload = malloc(size + sizeof(size_t));
        memcpy(payload, other.payload, cursor);
        cursor = other.cursor;
    }
}

Message::Message(Message&& other) noexcept:
        payload(other.payload), cursor(other.cursor) {
    other.payload = nullptr;
}

Message::Message(void* _payload): payload(_payload), cursor(0) {}

Message::~Message() {
    if (payload != nullptr) {
        free(payload);
    }
}

Message& Message::operator=(const Message& other) {
    if (this == &other) {
        return *this;
    }
    if (payload != nullptr) {
        free(payload);
    }
    if (other.payload == nullptr) {
        payload = nullptr;
        cursor = 0;
    } else {
        auto size = other.getSize();
        payload = malloc(size + sizeof(size_t));
        memcpy(payload, other.payload, cursor);
        cursor = other.cursor;
    }
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

void* Message::raw() const {
    return payload;
}

size_t Message::getSize() const {
    return sizeof(size_t) + *static_cast<size_t*>(payload);
}

bool Message::isInvalid() const {
    return payload == nullptr;
}

}  // namespace interproc
}  // namespace kktest
