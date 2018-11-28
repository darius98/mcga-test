#ifndef KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_H_
#define KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_H_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {
namespace core_matchers {

class IsTrueMatcher: public Matcher {
public:
    bool matches(const bool& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsTrueMatcher isTrue;

class IsFalseMatcher: public Matcher {
public:
    bool matches(const bool& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsFalseMatcher isFalse;

}
}

#endif
