#ifndef KKTEST_UTILS_MESSAGE_H_
#define KKTEST_UTILS_MESSAGE_H_

#include <cstdlib>

namespace kktest {

class Message {
public:
    explicit Message(std::size_t size);
    Message(const Message& other);
    Message(Message&& other) noexcept;

    ~Message();

    Message& addBytes(void* bytes, size_t numBytes);

    template<class T>
    Message& add(const T& obj) {
        return addBytes(&obj, sizeof(obj));
    }

    template<class T>
    Message& operator<<(const T& obj) {
        return addBytes(&obj, sizeof(obj));
    }
private:
    void* payload;
    std::size_t cursor;
};

class MessageSerializable {
public:
    virtual std::size_t numBytes() const = 0;

    virtual void serialize(Message& message) const = 0;

    Message toMessage() const;
};

}

#endif
