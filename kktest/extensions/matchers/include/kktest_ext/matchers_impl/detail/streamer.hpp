#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_DETAIL_STREAMER_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_DETAIL_STREAMER_HPP_

#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace kktest {
namespace matchers {
namespace detail {

template<class S, class = void>
class Streamer {
 public:
    static void Send(std::stringstream& stream, const S& obj) {
        Format(stream, obj);
    }

 private:
    template<class T>
    static void Format(std::stringstream& s, const std::vector<T>& obj) {
        FormatList<T, std::vector<T>>(s, obj);
    }

    template<class T>
    static void Format(std::stringstream& s, const std::list<T>& obj) {
        FormatList<T, std::list<T>>(s, obj);
    }

    template<class T>
    static void Format(std::stringstream& s, const std::deque<T>& obj) {
        FormatList<T, std::deque<T>>(s, obj);
    }

    template<class T>
    static void Format(std::stringstream& s, const std::set<T>& obj) {
        FormatList<T, std::set<T>>(s, obj, '{', '}');
    }

    template<class T>
    static void Format(std::stringstream& s, const std::unordered_set<T>& obj) {
        FormatList<T, std::unordered_set<T>>(s, obj, '{', '}');
    }

    template<class K, class V>
    static void Format(std::stringstream& s, const std::map<K, V>& obj) {
        FormatMap<K, V, std::map<K, V>>(s, obj);
    }

    template<class K, class V>
    static void Format(std::stringstream& s,
                       const std::unordered_map<K, V>& obj) {
        FormatMap<K, V, std::unordered_map<K, V>>(s, obj);
    }

    template<class I, class T>
    static void FormatList(std::stringstream& s, const T& obj,
                           char start = '[',
                           char finish = ']') {
        s << start;
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            if (it != obj.begin()) {
                s << ", ";
            }
            Streamer<I>::Send(s, *it);
        }
        s << finish;
    }

    template<class K, class V, class T>
    static void FormatMap(std::stringstream& s, const T& obj) {
        bool first = true;
        s << "{";
        for (const std::pair<K, V>& entry : obj) {
            if (first) {
                first = false;
            } else {
                s << ", ";
            }
            Streamer<K>::Send(s, entry.first);
            s << ":";
            Streamer<V>::Send(s, entry.second);
        }
        s << "}";
    }

    /**
     * In case all else fails.
     */
    template<class T>
    static void Format(std::stringstream& s, const T&) {
        s << "[UNPRINTABLE OBJECT]";
    }
};

template<class...> using void_t = void;

template<class S>
class Streamer<S, void_t<decltype(std::cout << std::declval<S>())>> {
 public:
    static void Send(std::stringstream& s, const S& obj) {
        s << obj;
    }

    static void Send(std::stringstream& s, const S* obj) {
        s << obj;
    }
};

template<>
class Streamer<std::string,
                void_t<decltype(std::declval<std::ostream>()
                                       << std::declval<std::string>())
                      >
                > {
 public:
    static void Send(std::stringstream& s, const std::string& obj) {
        std::string objCopy = obj;
        std::size_t pos = 0;
        while ((pos = objCopy.find('\n', pos)) != std::string::npos) {
            objCopy.replace(pos, 1, "\\n");
            pos += 2;
        }
        pos = 0;
        while ((pos = objCopy.find('\t', pos)) != std::string::npos) {
            objCopy.replace(pos, 1, "\\t");
            pos += 2;
        }
        pos = 0;
        while ((pos = objCopy.find('\r', pos)) != std::string::npos) {
            objCopy.replace(pos, 1, "\\r");
            pos += 2;
        }
        s << objCopy;
    }
};

}
}
}

#endif
