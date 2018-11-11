#ifndef KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_
#define KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_

#include <utils/message.hpp>


namespace kktest {

class Pipe {
public:
    explicit Pipe(const int& _outputFD);

    void pipe(const Message& messageSerializable) const;

private:
    int outputFD;
};

}

#endif
