#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_STRING_MATCHERS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_STRING_MATCHERS_H_

#include <string>

#include <kktest_public/core/matcher.hpp>


namespace kktest {

class CharInStringMatcher: public Matcher {
public:
    explicit CharInStringMatcher(const char* _container,
                                 const char* _expectation=nullptr) noexcept;

    bool matches(const char& ch);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
private:
    const char* container;
    const char* expectation;
};

extern CharInStringMatcher* isLetter;

extern CharInStringMatcher* isDigit;

extern CharInStringMatcher* isLowercaseLetter;

extern CharInStringMatcher* isUppercaseLetter;

extern CharInStringMatcher* isBinaryDigit;

extern CharInStringMatcher* isOctDigit;

extern CharInStringMatcher* isHexDigit;

extern CharInStringMatcher* isWhitespace;

class IsSubstringMatcher: public Matcher {
public:
    explicit IsSubstringMatcher(std::string _container);

    bool matches(const std::string& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
private:
    std::string container;
};

IsSubstringMatcher* isSubstringOf(const std::string& s);

}

#endif
