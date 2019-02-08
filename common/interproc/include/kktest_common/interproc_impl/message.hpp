#ifndef KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_HPP_
#define KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_HPP_

#include <functional>

#include <kktest_common/string.hpp>

namespace kktest {
namespace interproc {

class Message {
 public:
    static const Message INVALID;

    template<class... Args>
    static Message build(const Args... args) {
        BytesCounter counter;
        counter.add(args...);
        Builder messageBuilder(counter.getNumBytesConsumed());
        messageBuilder.add(args...);
        return messageBuilder.build();
    }

    static Message read(const void* src, std::size_t maxSize);

    Message(const Message& other);
    Message(Message&& other) noexcept;

    ~Message();

    Message& operator=(const Message& other);
    Message& operator=(Message&& other) noexcept;

    bool operator==(const Message& other) const;

    std::size_t getSize() const;

    bool isInvalid() const;

    Message& operator<<(String& obj);

    template<class T>
    Message& operator<<(T& obj) {
        obj = *reinterpret_cast<T*>(
                    static_cast<std::uint8_t*>(payload) + readHead);
        readHead += sizeof(obj);
        return *this;
    }

 private:
    explicit Message(void* _payload) noexcept;

    void* payload;
    std::size_t readHead = sizeof(std::size_t);

    // helper internal classes

    class BytesConsumer {
     public:
        virtual BytesConsumer& addBytes(const void* bytes,
                                        std::size_t numBytes) = 0;

        BytesConsumer& add(const String& obj);

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
        BytesCounter& addBytes(const void* bytes,
                               std::size_t numBytes) override;

        std::size_t getNumBytesConsumed() const;

     private:
        std::size_t bytesConsumed = 0;
    };


    class Builder: public BytesConsumer {
     public:
        explicit Builder(std::size_t size);

        Builder& addBytes(const void* bytes, std::size_t numBytes) override;

        Message build();

     private:
        void* payloadBuilder;
        std::size_t cursor;
    };

    friend class PipeReader;
    friend class PipeWriter;
};

}
}

#endif
