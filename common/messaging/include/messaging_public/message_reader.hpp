#ifndef COMMON_MESSAGING_INCLUDE_MESSAGING_PUBLIC_MESSAGE_READER_H_
#define COMMON_MESSAGING_INCLUDE_MESSAGING_PUBLIC_MESSAGE_READER_H_

#include "message.hpp"


namespace messaging {

class MessageReader {
public:
    explicit MessageReader(const Message& _message);

    template<class T>
    T read() {
        T obj = *((T*)((std::uint8_t*)message.getPayload() + cursor));
        cursor += sizeof(T);
        return obj;
    }

private:
    int cursor = sizeof(std::size_t);
    const Message& message;
};

}

#endif
