#ifndef RUNTIME_TESTING_TESTS_MATCHERS_MATCHER_TEST_H_
#define RUNTIME_TESTING_TESTS_MATCHERS_MATCHER_TEST_H_

#include <iostream>

#include "../../src/matcher/matcher.hpp"

static int numFailedTests;

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
        std::cout << "\t Actual  description: " << actualDescription << "\n";
    }
    return ret;
}

template<class T>
void test(const std::string& message,
          const T& value,
          matcher::Matcher<T>* matcher,
          bool shouldMatch=true,
          const std::string& expectedDescription="") {
    std::cout << message << ": ";
    numFailedTests +=
            !expectMatch(value, matcher, shouldMatch, expectedDescription);
}

void iterableMatcherTest();
void comparisonMatcherTest();
void truthMatcherTest();

#endif
