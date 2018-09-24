#ifndef RUNTIME_TESTING_MATCHER_TRUTH_MATCHER_H_
#define RUNTIME_TESTING_MATCHER_TRUTH_MATCHER_H_

#include "matcher.hpp"


namespace matcher {

template<bool b>
class TruthMatcher : public Matcher<bool> {
public:
    bool matches(const bool& object) override {
        return object == b;
    }

    void describe(const bool& object, Description* description) override {
        if (b) {
            description->append("true, got false");
        } else {
            description->append("false, got true");
        }
    }
};

} // namespace matcher

#endif
