#ifndef COMMON_MESSAGING_INCLUDE_OUTPUT_PIPE_H_
#define COMMON_MESSAGING_INCLUDE_OUTPUT_PIPE_H_

#include "message.hpp"


namespace messaging {

class OutputPipe {
public:
    explicit OutputPipe(const int& _outputFD);

    void close();

    void pipe(const Message& message) const;

private:
    int outputFD;
};

}

#endif
