#ifndef MESSAGING_LIB_INCLUDE_MESSAGE_H_
#define MESSAGING_LIB_INCLUDE_MESSAGE_H_

#include <functional>
#include <string>


namespace messaging {

class BytesConsumer {
public:
    virtual BytesConsumer& addBytes(const void* bytes, std::size_t numBytes) = 0;

    template<class T>
    BytesConsumer& add(const T& obj) {
        return addBytes(&obj, sizeof(obj));
    }

    template<class T>
    BytesConsumer& operator<<(const T& obj) {
        return add(obj);
    }
};

class BytesCounter: public BytesConsumer {
public:
    BytesCounter& addBytes(const void* bytes, std::size_t numBytes) override;

    std::size_t getNumBytesConsumed() const;

private:
    std::size_t bytesConsumed = 0;
};

class Message: public BytesConsumer {
public:
    static Message build(const std::function<void(BytesConsumer&)>& builder);

    static std::size_t isSane(const void* ptr, std::size_t size);

    explicit Message(std::size_t size);
    Message(const Message& other);
    Message(Message&& other) noexcept;
    explicit Message(void* _payload);

    ~Message();

    Message& operator=(const Message& other);
    Message& operator=(Message&& other) noexcept;

    Message& addBytes(const void* bytes, size_t numBytes) override;

    void* getPayload() const;

    std::size_t getSize() const;

private:
    void* payload;
    std::size_t cursor;
};

}

#endif
