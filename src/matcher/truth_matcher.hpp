#ifndef MATCHER_TRUTH_MATCHER_H_
#define MATCHER_TRUTH_MATCHER_H_

#include "matcher.hpp"

namespace testing {
namespace matcher {

template<bool b>
class TruthMatcher : public Matcher<bool> {
public:
    bool matches(const bool& object) const override {
        return object == b;
    }

    void
    describe(const bool& object, std::stringstream* description) const;
};

template<>
void TruthMatcher<true>::describe(const bool& object,
                                  std::stringstream* description) const {
    (*description) << "expected true, got false";
}

template<>
void TruthMatcher<false>::describe(const bool& object,
                                   std::stringstream* description) const {
    (*description) << "expected false, got true";
}


static const Matcher<bool>* isTrue = new TruthMatcher<true>();

static const Matcher<bool>* isFalse = new TruthMatcher<false>();

} // namespace matcher
} // namespace testing

#endif
