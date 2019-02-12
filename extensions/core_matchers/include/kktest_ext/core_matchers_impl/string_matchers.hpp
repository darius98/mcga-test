#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_STRING_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_STRING_MATCHERS_HPP_

#include <string>

#include <kktest_ext/core_matchers_impl/matcher.hpp>
#include <kktest_ext/core_matchers_impl/detail/decl.hpp>

namespace kktest {
namespace core_matchers {

extern detail::CharInStringMatcher isLetter;

extern detail::CharInStringMatcher isDigit;

extern detail::CharInStringMatcher isLowercaseLetter;

extern detail::CharInStringMatcher isUppercaseLetter;

extern detail::CharInStringMatcher isBinaryDigit;

extern detail::CharInStringMatcher isOctDigit;

extern detail::CharInStringMatcher isHexDigit;

extern detail::CharInStringMatcher isWhitespace;

detail::IsSubstringMatcher isSubstringOf(const std::string& s);

namespace detail {

class CharInStringMatcher: public Matcher {
 public:
    explicit CharInStringMatcher(const char* _container,
                                 const char* _expectation = nullptr) noexcept;

    bool matches(const char& ch);

    void describe(Description* description);

    void describeMismatch(Description* description, const char& ch);

 private:
    const char* container;
    const char* expectation;
};

class IsSubstringMatcher: public Matcher {
 public:
    explicit IsSubstringMatcher(std::string _container);

    bool matches(const std::string& object);

    void describe(Description* description);

 private:
    std::string container;
};

}
}
}

#endif
