#ifndef KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_
#define KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_

#include <utils/message.hpp>


namespace kktest {

class Pipe {
public:
    explicit Pipe(const int& _outputFD);

    void pipe(const MessageSerializable* messageSerializable) const;

private:
    void writeBytes(const void* bytes, std::size_t numBytes) const;

    int outputFD;
};

}

#endif
