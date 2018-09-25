#ifndef RUNTIME_TESTING_MATCHER_DESCRIPTION_H_
#define RUNTIME_TESTING_MATCHER_DESCRIPTION_H_

#include <sstream>
#include <string>

#include "description_streamer.hpp"


namespace matcher {

class Description {
public:
    static Description* createForExpectation();

    template<class T>
    Description* append(T obj) {
        DescriptionStreamer<T>::send(this->stream, obj);
        return this;
    }

    template<class T, class... Args>
    Description* append(T obj, const Args... args) {
        this->append(obj);
        this->append(args...);
        return this;
    }

    std::string toString() const;
private:
    std::stringstream stream;
};

}

#endif
