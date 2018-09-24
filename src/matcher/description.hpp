#ifndef MATCHER_DESCRIPTION_H_
#define MATCHER_DESCRIPTION_H_

#include <iostream>
#include <sstream>
#include <string>


namespace matcher {

class Description {
public:
    template<class T>
    Description* append(T obj) {
        Stream<T>::send(this->stream, obj);
        return this;
    }

    template<class T, class... Args>
    Description* append(T obj, const Args... args) {
        this->append(obj);
        this->append(args...);
        return this;
    }

    std::string toString() const {
        return this->stream.str();
    }
private:
    std::stringstream stream;

    template<class T, class=void>
    struct Stream {
        static void send(std::stringstream& stream, T obj) {
            stream << "value at " << &obj;
        }
    };

    template<class T>
    struct Stream<T, std::void_t<decltype(std::cout << std::declval<T>())>> {
        static void send(std::stringstream& stream, T obj) {
            stream << obj;
        }
    };
};

}

#endif
