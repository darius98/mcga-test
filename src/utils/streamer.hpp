#ifndef RUNTIME_TESTING_MATCHER_DESCRIPTION_STREAMER_H_
#define RUNTIME_TESTING_MATCHER_DESCRIPTION_STREAMER_H_

#include <deque>
#include <iostream>
#include <list>
#include <sstream>
#include <set>
#include <unordered_set>
#include <vector>


namespace runtime_testing {
namespace utils {

template<class S, class=void>
struct Streamer {
    static void send(std::stringstream& stream, S obj) {
        format(stream, obj);
    }

private:
    template<class T>
    static void format(std::stringstream& s, const std::vector<T>& obj) {
        formatList<T, std::vector<T>>(s, obj);
    }

    template<class T>
    static void format(std::stringstream& s, const std::list<T>& obj) {
        formatList<T, std::list<T>>(s, obj);
    }

    template<class T>
    static void format(std::stringstream& s, const std::deque<T>& obj) {
        formatList<T, std::deque<T>>(s, obj);
    }

    template<class T>
    static void format(std::stringstream& s, const std::set<T>& obj) {
        formatList<T, std::set<T>>(s, obj, '{', '}');
    }

    template<class T>
    static void format(std::stringstream& s, const std::unordered_set<T>& obj) {
        formatList<T, std::unordered_set<T>>(s, obj, '{', '}');
    }

    template<class I, class T>
    static void formatList(std::stringstream& s,
                           T obj,
                           char start='[',
                           char finish=']') {
        s << start;
        for (auto it = obj.begin(); it != obj.end(); ++ it) {
            if (it != obj.begin()) {
                s << ", ";
            }
            Streamer<I>::send(s, *it);
        }
        s << finish;
    }

    /// In case all else fails
    template<class T>
    static void format(std::stringstream& s, T obj) {
        s << "*(" << &obj << ")";
    }
};

template<class S>
struct Streamer
        <S, std::void_t<decltype(std::cout << std::declval<S>())>> {
    static void send(std::stringstream& s, S obj) {
        s << obj;
    }
};

}
}

#endif
