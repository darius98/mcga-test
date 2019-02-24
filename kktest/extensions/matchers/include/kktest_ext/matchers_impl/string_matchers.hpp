#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_STRING_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_STRING_MATCHERS_HPP_

#include <string>

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

KKTEST_EXPORT extern detail::CharInStringMatcher isLetter;

KKTEST_EXPORT extern detail::CharInStringMatcher isDigit;

KKTEST_EXPORT extern detail::CharInStringMatcher isLowercaseLetter;

KKTEST_EXPORT extern detail::CharInStringMatcher isUppercaseLetter;

KKTEST_EXPORT extern detail::CharInStringMatcher isBinaryDigit;

KKTEST_EXPORT extern detail::CharInStringMatcher isOctDigit;

KKTEST_EXPORT extern detail::CharInStringMatcher isHexDigit;

KKTEST_EXPORT extern detail::CharInStringMatcher isWhitespace;

KKTEST_EXPORT detail::IsSubstringMatcher isSubstringOf(const std::string& s);

namespace detail {

class KKTEST_EXPORT CharInStringMatcher: public Matcher {
 public:
    explicit CharInStringMatcher(const char* container,
                                 const char* expectation = nullptr) noexcept;

    bool matches(const char& ch);

    void describe(Description* description);

    void describeMismatch(Description* description, const char& ch);

 private:
    const char* container;
    const char* expectation;
};

class KKTEST_EXPORT IsSubstringMatcher: public Matcher {
 public:
    explicit IsSubstringMatcher(std::string container);

    bool matches(const std::string& object);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& object);

 private:
    std::string container;
};

}
}
}

#endif
