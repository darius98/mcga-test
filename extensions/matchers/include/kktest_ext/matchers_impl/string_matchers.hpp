#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_STRING_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_STRING_MATCHERS_HPP_

#include <string>

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

extern KKTEST_API detail::CharInStringMatcher isLetter;

extern KKTEST_API detail::CharInStringMatcher isDigit;

extern KKTEST_API detail::CharInStringMatcher isLowercaseLetter;

extern KKTEST_API detail::CharInStringMatcher isUppercaseLetter;

extern KKTEST_API detail::CharInStringMatcher isBinaryDigit;

extern KKTEST_API detail::CharInStringMatcher isOctDigit;

extern KKTEST_API detail::CharInStringMatcher isHexDigit;

extern KKTEST_API detail::CharInStringMatcher isWhitespace;

KKTEST_API detail::IsSubstringMatcher isSubstringOf(const std::string& s);

namespace detail {

class KKTEST_API CharInStringMatcher: public Matcher {
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

class KKTEST_API IsSubstringMatcher: public Matcher {
 public:
    explicit IsSubstringMatcher(std::string _container);

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
