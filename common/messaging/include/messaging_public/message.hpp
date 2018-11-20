#ifndef COMMON_MESSAGING_INCLUDE_MESSAGING_PUBLIC_MESSAGE_H_
#define COMMON_MESSAGING_INCLUDE_MESSAGING_PUBLIC_MESSAGE_H_

#include <functional>
#include <string>


namespace messaging {

class BytesConsumer {
public:
    virtual BytesConsumer& addBytes(const void* bytes, std::size_t numBytes) = 0;

    BytesConsumer& add(const std::string& obj);

    template<class T>
    BytesConsumer& add(const T& obj) {
        return addBytes(&obj, sizeof(obj));
    }

    template<class T1, class T2, class... Args>
    BytesConsumer& add(const T1& a1, const T2& a2, const Args... args) {
        add(a1);
        return add(a2, args...);
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

class Message: private BytesConsumer {
public:
    template<class... Args>
    static Message build(const Args... args) {
            BytesCounter counter;
            counter.add(args...);
            Message message(counter.getNumBytesConsumed());
            message.add(args...);
            return message;
    }

    static std::size_t isSane(const void* ptr, std::size_t size);

    explicit Message(std::size_t size);
    Message(const Message& other);
    Message(Message&& other) noexcept;
    explicit Message(void* _payload);

    ~Message();

    Message& operator=(const Message& other);
    Message& operator=(Message&& other) noexcept;

    void* getPayload() const;

    std::size_t getSize() const;

private:
    Message& addBytes(const void* bytes, size_t numBytes) override;

    void* payload;
    std::size_t cursor;
};

}

#endif
