#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_HPP_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {
namespace core_matchers {

class IsTrueMatcher: public Matcher {
 public:
    bool matches(const bool& object);

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

extern IsTrueMatcher isTrue;

class IsFalseMatcher: public Matcher {
 public:
    bool matches(const bool& object);

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

extern IsFalseMatcher isFalse;

}  // namespace core_matchers
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_HPP_
