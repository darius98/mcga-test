#ifndef MATCHERS_MATCHER_TEST_HPP
#define MATCHERS_MATCHER_TEST_HPP

#include <iostream>

#include "../../src/matcher/matcher.hpp"

template<class T>
bool expectMatch(const T& value,
                 matcher::Matcher<T>* matcher,
                 bool shouldMatch=true,
                 const std::string& expectedDescription="") {
    bool matches = matcher->matches(value);
    if (matches) {
        std::cout << (shouldMatch ? "passed." : "failed.") << std::endl;
        return shouldMatch;
    }
    if (shouldMatch) {
        std::cout << "failed." << std::endl;
        return false;
    }
    if (expectedDescription.empty()) {
        std::cout << "passed." << std::endl;
        return true;
    }
    auto d = new matcher::Description();
    matcher->describe(value, d);
    std::string actualDescription = d->toString();
    delete d;
    bool ret = (actualDescription == expectedDescription);
    std::cout << (ret ? "passed." : "failed.") << std::endl;
    if (!ret) {
        std::cout << "\tExpected description: " << expectedDescription << "\n";
        std::cout << "\tActual description: " << actualDescription << "\n";
    }
    return ret;
}

#endif
