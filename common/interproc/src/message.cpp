#include "common/interproc/src/message.hpp"

#include <cstdlib>
#include <cstring>

using namespace std;

namespace kktest {
namespace interproc {

const Message Message::INVALID = Message(nullptr);

Message Message::read(const void* src, size_t maxSize) {
    if (maxSize < sizeof(size_t)) {
        return INVALID;
    }
    auto expectedSize = (*static_cast<const size_t*>(src)) + sizeof(size_t);
    if (expectedSize > maxSize) {
        return INVALID;
    }
    void* messagePayload = malloc(expectedSize);
    memcpy(messagePayload, src, expectedSize);
    return Message(messagePayload);
}

Message::Message(): Message(INVALID) {}

Message::Message(const Message& other) {
    copyContent(other);
}

Message::Message(Message&& other) noexcept: payload(other.payload) {
    if (this != &other) {
        other.payload = nullptr;
    }
}

Message::Message(void* _payload) noexcept: payload(_payload) {}

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
    copyContent(other);
    readHead = sizeof(size_t);
    return *this;
}

Message& Message::operator=(Message&& other) noexcept {
    readHead = sizeof(size_t);
    if (this == &other) {
        return *this;
    }
    payload = other.payload;
    other.payload = nullptr;
    return *this;
}

void Message::copyContent(const Message& other) {
    if (other.isInvalid()) {
        payload = nullptr;
    } else {
        auto size = other.getSize();
        payload = malloc(size);
        memcpy(payload, other.payload, size);
    }
}

size_t Message::getSize() const {
    return sizeof(size_t) + *static_cast<size_t*>(payload);
}

bool Message::operator==(const Message& other) const {
    return this == &other || (isInvalid() && other.isInvalid());
}

bool Message::isInvalid() const {
    return payload == nullptr;
}

template<>
const Message& Message::operator>>(String& obj) const {
    decltype(obj.size()) size;
    (*this) >> size;
    obj.assign(static_cast<char*>(payload) + readHead,
               static_cast<char*>(payload) + readHead + size);
    readHead += size;
    return *this;
}

template<>
Message::BytesConsumer& Message::BytesConsumer::add(const String& obj) {
    add(obj.size());
    addBytes(obj.c_str(), obj.size());
    return *this;
}

Message::BytesCounter& Message::BytesCounter::addBytes(const void*,
                                                       size_t numBytes) {
    bytesConsumed += numBytes;
    return *this;
}

size_t Message::BytesCounter::getNumBytesConsumed() const {
    return bytesConsumed;
}

Message::Builder::Builder(size_t size):
        payloadBuilder(malloc(size + sizeof(size_t))) {
    memcpy(payloadBuilder, &size, sizeof(size_t));
    cursor = sizeof(size_t);
}

Message::Builder& Message::Builder::addBytes(const void* bytes,
                                             size_t numBytes) {
    memcpy(static_cast<uint8_t*>(payloadBuilder) + cursor, bytes, numBytes);
    cursor += numBytes;
    return *this;
}

Message Message::Builder::build() {
    return Message(payloadBuilder);
}

}
}
