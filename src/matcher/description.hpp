#ifndef KKTEST_MATCHER_DESCRIPTION_H_
#define KKTEST_MATCHER_DESCRIPTION_H_

#include <sstream>
#include <string>

#include "utils/streamer.hpp"


namespace kktest {

class Description {
public:
    static Description* createForExpect(
            const std::string& file,
            int line,
            const std::string& extra);

    template<class T>
    Description& operator<<(T obj) {
        utils::Streamer<T>::send(stream, obj);
        return *this;
    }

    template<class T>
    Description& appendType() {
        utils::Streamer<T>::sendType(stream);
    }

    std::string toString() const;
private:
    std::stringstream stream;
};

}

#endif
