#ifndef RUNTIME_TESTING_MATCHER_DESCRIPTION_H_
#define RUNTIME_TESTING_MATCHER_DESCRIPTION_H_

#include <sstream>
#include <string>

#include "src/utils/streamer.hpp"


namespace matcher {

class Description {
public:
    static Description* createForExpectation(
            const char* fileName,
            const int& lineNumber,
            const std::string& extra);

    template<class T>
    Description* append(T obj) {
        runtime_testing::utils::Streamer<T>::send(stream, obj);
        return this;
    }

    template<class T, class... Args>
    Description* append(T obj, const Args... args) {
        append(obj);
        append(args...);
        return this;
    }

    template<class T>
    Description* appendType() {
        runtime_testing::utils::Streamer<T>::sendType(stream);
    }

    std::string toString() const;
private:
    std::stringstream stream;
};

}

#endif
