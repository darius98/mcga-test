#ifndef RUNTIME_TESTING_MATCHER_DESCRIPTION_H_
#define RUNTIME_TESTING_MATCHER_DESCRIPTION_H_

#include <sstream>
#include <string>

#include "src/utils/streamer.hpp"


namespace matcher {

class Description {
public:
    static Description* createForExpect(
            const std::string& file,
            int line,
            const std::string& extra);

    template<class T>
    Description& operator<<(T obj) {
        runtime_testing::utils::Streamer<T>::send(stream, obj);
        return *this;
    }

    template<class T>
    Description& appendType() {
        runtime_testing::utils::Streamer<T>::sendType(stream);
    }

    std::string toString() const;
private:
    std::stringstream stream;
};

}

#endif
