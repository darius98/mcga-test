#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_DETAIL_STREAMER_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_DETAIL_STREAMER_HPP_

#ifdef KKTEST_USE_ABI_DEMANGLE
#include <cxxabi.h>
#endif

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

#include <kktest_common/string.hpp>

namespace kktest {
namespace core_matchers {
namespace detail {

template<class S, class = void>
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

    template<class T>
    static void formatType(std::stringstream& s) {
        int stat;
        String rawName = typeid(T).name();
#ifdef KKTEST_USE_ABI_DEMANGLE
        char* name = abi::__cxa_demangle(rawName.c_str(),
                                         nullptr,
                                         nullptr,
                                         &stat);
        if (stat == 0) {
            rawName = name;
            free(name);
        }
#endif
        s << rawName;
    }

    /**
     * In case all else fails.
     */
    template<class T>
    static void format(std::stringstream& s, T) {
        s << "[OBJECT OF TYPE ";
        Streamer<T>::sendType(s);
        s << "]";
    }
};

template<class...> using void_t = void;

template<class S>
struct Streamer<S, void_t<decltype(std::cout << std::declval<S>())>> {
public:
    static void sendType(std::stringstream& stream) {
        formatType<S>(stream);
    }

    static void send(std::stringstream& s, S obj) {
        s << obj;
    }

private:
    template<class T>
    static void formatType(std::stringstream& s) {
        int stat;
        String rawName = typeid(T).name();
#ifdef KKTEST_USE_ABI_DEMANGLE
        char* name = abi::__cxa_demangle(rawName.c_str(),
                                         nullptr,
                                         nullptr,
                                         &stat);
        if (stat == 0) {
            rawName = name;
            free(name);
        }
#endif
        s << rawName;
    }
};

template<>
struct Streamer<std::string,
                void_t<decltype(std::declval<std::ostream>()
                                       << std::declval<std::string>())
                      >
                > {
public:
    static void sendType(std::stringstream& stream) {
        formatType<std::string>(stream);
    }

    static void send(std::stringstream& s, std::string obj) {
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        while ((pos = obj.find('\n', pos)) != String::npos) {
            obj.replace(pos, 1, "\\n");
            pos += 2;
        }
        pos = 0;
        while ((pos = obj.find('\t', pos)) != String::npos) {
            obj.replace(pos, 1, "\\t");
            pos += 2;
        }
        pos = 0;
        while ((pos = obj.find('\r', pos)) != String::npos) {
            obj.replace(pos, 1, "\\r");
            pos += 2;
        }
        s << obj;
    }

private:
    template<class T>
    static void formatType(std::stringstream& s) {
        int stat;
        String rawName = typeid(T).name();
#ifdef KKTEST_USE_ABI_DEMANGLE
        char* name = abi::__cxa_demangle(rawName.c_str(),
                                         nullptr,
                                         nullptr,
                                         &stat);
        if (stat == 0) {
            rawName = name;
            free(name);
        }
#endif
        s << rawName;
    }
};

}
}
}

#endif
