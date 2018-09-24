#ifndef MATCHER_TRUTH_MATCHER_H_
#define MATCHER_TRUTH_MATCHER_H_

#include "matcher.hpp"


namespace matcher {

template<bool b>
class TruthMatcher : public Matcher<bool> {
public:
    bool matches(const bool& object) override {
        return object == b;
    }

    void describe(const bool& object, Description* description) override;
};

template<>
void TruthMatcher<true>::describe(const bool& object,
                                  Description* description) {
    description->append("true, got false");
}

template<>
void TruthMatcher<false>::describe(const bool& object,
                                   Description* description) {
    description->append("false, got true");
}

} // namespace matcher

#endif
