#pragma once

#include <sstream>

#include <kktest.hpp>
#include <kktest_ext/matchers/detail/type_helpers.hpp>

namespace kktest::matchers {

class Description {
 public:
    Description() = default;
    Description(const Description& other): stream(other.stream.str()) {}

    template<class T>
    Description& operator<<(const T& obj) {
        if constexpr (tp::IsStringLike<T>) {
            appendString(obj);
        } else if constexpr (tp::IsIterable<T>) {
            appendIterable(obj);
        } else if constexpr (tp::IsPair<T>) {
            appendPair(obj);
        } else if constexpr (tp::IsTuple<T>) {
            appendTuple(obj);
        } else if constexpr (tp::IsNullptrT<T>) {
            stream << (int*)obj;
        } else {
            stream << obj;
        }
        return *this;
    }

    Description& appendRawString(const std::string& str) {
        stream << str;
        return *this;
    }

    std::string toString() const {
        return stream.str();
    }

 private:
    void appendString(std::string obj) {
        std::size_t pos = 0;
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
        stream << obj;
    }

    template<class T>
    void appendIterable(const T& obj) {
        stream << '[';
        bool first = true;
        for (const auto& item : obj) {
            if (first) {
                first = false;
            } else {
                stream << ',';
            }
            *this << item;
        }
        stream << ']';
    }

    template<class A, class B>
    void appendPair(const std::pair<A, B>& p) {
        stream << '(';
        (*this) << p.first;
        stream << ", ";
        (*this) << p.second;
        stream << ')';
    }

    template<class... Items>
    void appendTuple(const std::tuple<Items...>& t) {
        constexpr std::size_t size = std::tuple_size_v<Items...>;
        stream << '(';
        tp::StreamTuple(*this, t);
        stream << ')';
    }

    std::stringstream stream;
};


class Matcher {};

template<class S>
class StatefulMatcher : public Matcher {
 public:
    static constexpr bool HasState = true;

    typedef S State;

//    template<class T>
//    virtual bool matches(const T& obj, S* state) const = 0;
//
//    virtual void describe(Description* description) const = 0;
//
//    virtual void describeFailure(Description* description, S* state) const = 0;
};

class StatelessMatcher : public Matcher {
 public:
    static constexpr bool HasState = false;

    typedef int State; // for easier usage of nested matchers.

//    template<class T>
//    virtual bool matches(const T& obj) const = 0;
//
//    virtual void describe(Description* description) const = 0;
//
//    virtual void describeFailure(Description* description) const = 0;
};

namespace detail {

template<class T, class M>
bool __matches(const M& matcher, typename M::State* state, const T& obj) {
    if constexpr (M::HasState) {
        return matcher.matches(obj, state);
    } else {
        return matcher.matches(obj);
    }
}

template<class M>
void __describeFailure(Description* description,
                       const M& matcher,
                       typename M::State* state) {
    if constexpr (M::HasState) {
        matcher.describeFailure(description, state);
    } else {
        matcher.describeFailure(description);
    }
}

}

template<class T, class M>
void expect(const T& obj, M matcher) {
    static_assert(std::is_base_of_v<matchers::Matcher, M>);
    typename M::State state;
    if (detail::__matches(matcher, &state, obj)) {
        return;
    }
    Description description;
    description.appendRawString("Expected ");
    matcher.describe(&description);
    description.appendRawString("\n\tGot      '");
    description << obj;
    description.appendRawString("'\n\tWhich is ");
    detail::__describeFailure(&description, matcher, &state);
    fail("Expectation failed:\n\t" + description.toString());
}

}
