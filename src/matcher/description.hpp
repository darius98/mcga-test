#ifndef RUNTIME_TESTING_MATCHER_DESCRIPTION_H_
#define RUNTIME_TESTING_MATCHER_DESCRIPTION_H_

#include <cxxabi.h>

#include <sstream>
#include <string>

#include "src/utils/streamer.hpp"


namespace matcher {

class Description {
public:
    static Description* createForExpectation(
            const char* fileName,
            const int& lineNumber,
            const char* extra);

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
        int stat;
        std::string rawName = typeid(T).name();
        char* name = abi::__cxa_demangle(
                rawName.c_str(), nullptr, nullptr, &stat
        );
        if(stat == 0) {
            rawName = name;
            free(name);
        }
        this->append(rawName);
    }

    std::string toString() const;
private:
    std::stringstream stream;
};

}

#endif
