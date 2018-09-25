#ifndef RUNTIME_TESTING_MATCHER_TRUTH_MATCHER_H_
#define RUNTIME_TESTING_MATCHER_TRUTH_MATCHER_H_

#include "matcher.hpp"


namespace matcher {

class IsTrueMatcher: public BaseMatcher {
public:
    bool matches(const bool& object);

    void describe(const bool& object, Description* description);
};

class IsFalseMatcher: public BaseMatcher {
public:
    bool matches(const bool& object);

    void describe(const bool& object, Description* description);
};

} // namespace matcher

#endif
