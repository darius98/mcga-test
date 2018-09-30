#ifndef RUNTIME_TESTING_MATCHER_STRING_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_STRING_MATCHERS_H_

#include <string>

#include "matcher.hpp"


namespace matcher {

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

class IsSubstringMatcher: public Matcher {
public:
    explicit IsSubstringMatcher(std::string _container);

    bool matches(const std::string& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
private:
    std::string container;
};

}

#endif
