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

    template<class K, class V>
    static void format(std::stringstream& s, const std::map<K, V>& obj) {
        formatMap<K, V, std::map<K, V>>(s, obj);
    }

    template<class K, class V>
    static void format(std::stringstream& s,
                       const std::unordered_map<K, V>& obj) {
        formatMap<K, V, std::unordered_map<K, V>>(s, obj);
    }

    template<class I, class T>
    static void formatList(std::stringstream& s, T obj,
                           char start = '[',
                           char finish = ']') {
        s << start;
        for (auto it = obj.begin(); it != obj.end(); ++it) {
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
        for (const std::pair<K, V>& entry : obj) {
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
    }

    /**
     * In case all else fails.
     */
    template<class T>
    static void format(std::stringstream& s, T) {
        s << "[UNPRINTABLE OBJECT]";
    }
};

template<class...> using void_t = void;

template<class S>
class Streamer<S, void_t<decltype(std::cout << std::declval<S>())>> {
 public:
    static void send(std::stringstream& s, S obj) {
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
    static void send(std::stringstream& s, std::string obj) {
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        while ((pos = obj.find('\n', pos)) != std::string::npos) {
            obj.replace(pos, 1, "\\n");
            pos += 2;
        }
        pos = 0;
        while ((pos = obj.find('\t', pos)) != std::string::npos) {
            obj.replace(pos, 1, "\\t");
            pos += 2;
        }
        pos = 0;
        while ((pos = obj.find('\r', pos)) != std::string::npos) {
            obj.replace(pos, 1, "\\r");
            pos += 2;
        }
        s << obj;
    }
};

}
}
}

#endif
