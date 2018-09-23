#ifndef MATCHER_DESCRIPTION_H_
#define MATCHER_DESCRIPTION_H_

#include <sstream>
#include <string>

#include "../utils/has_operator.hpp"


namespace matcher {

class Description {
public:
    template<class T>
    Description* append(const T& obj) {
        if (testing::utils::hasLeftShift<std::stringstream, const T&>()) {
            this->stream << obj;
        } else {
            this->appendRaw(&obj);
        }
        return this;
    }

    template<class T1, class... Args>
    Description* append(const T1& obj, const Args... args) {
        this->append(obj);
        this->append(args...);
        return this;
    }

    std::string toString() const {
        return this->stream.str();
    }
private:
    void appendRaw(const void* obj) {
        this->stream << "value at " << obj;
    }

    std::stringstream stream;
};

}

#endif
