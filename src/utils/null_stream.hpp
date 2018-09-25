#ifndef RUNTIME_TESTING_TESTING_NOWHERE_STREAM_H_
#define RUNTIME_TESTING_TESTING_NOWHERE_STREAM_H_

#include <ostream>
#include <streambuf>


namespace runtime_testing {
namespace utils {

class NullStreamBuffer: public std::streambuf {
protected:
    int overflow(int c) override;
private:
    char buffer[64] = {0};
};

class NullStream: public NullStreamBuffer, public std::ostream {
public:
    NullStream() noexcept;
};

extern NullStream nullStream;

}
}

#endif
