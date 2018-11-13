#ifndef MESSAGING_LIB_INCLUDE_OUTPUT_PIPE_H_
#define MESSAGING_LIB_INCLUDE_OUTPUT_PIPE_H_

#include "message.hpp"


namespace messaging {

class OutputPipe {
public:
    explicit OutputPipe(const int& _outputFD);

    void pipe(const Message& message) const;

private:
    int outputFD;
};

}

#endif
