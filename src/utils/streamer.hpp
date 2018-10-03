#ifndef KTEST_UTILS_STREAMER_H_
#define KTEST_UTILS_STREAMER_H_

#include <cxxabi.h>

#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace ktest {
namespace utils {

template<class S, class=void>
struct Streamer {
    static void send(std::stringstream& stream, S obj) {
        format(stream, obj);
    }

    static void sendType(std::stringstream& stream) {
        formatType<S>(stream);
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

    template<class K, class V>
    static void format(std::stringstream& s, const std::map<K, V>& obj) {
        formatMap<K, V, std::map<K, V>>(s, obj);
    };

    template<class K, class V>
    static void format(std::stringstream& s,
                       const std::unordered_map<K, V>& obj) {
        formatMap<K, V, std::unordered_map<K, V>>(s, obj);
    };

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

    template<class K, class V, class T>
    static void formatMap(std::stringstream& s, const T& obj) {
        bool first = true;
        s << "{";
        for (const std::pair<K, V>& entry: obj) {
            if (first) {
                first = false;
            } else {
                s << ", ";
            }
            Streamer<K>::send(s, entry.first);
            s << ":";
            Streamer<V>::send(s, entry.second);
        }
        s << "}";
    };

    template<class T>
    static void formatType(std::stringstream& s) {
        int stat;
        std::string rawName = typeid(T).name();
        char* name = abi::__cxa_demangle(
                rawName.c_str(), nullptr, nullptr, &stat
        );
        if (stat == 0) {
            rawName = name;
            free(name);
        }
        s << rawName;
    }

    /**
     * In case all else fails.
     */
    template<class T>
    static void format(std::stringstream& s, T obj) {
        s << "[OBJECT OF TYPE ";
        Streamer<T>::sendType(s);
        s << "]";
    }
};

template<class S>
struct Streamer<S, std::void_t<decltype(std::cout << std::declval<S>())>> {
    static void send(std::stringstream& s, S obj) {
        s << obj;
    }
};

}
}

#endif
