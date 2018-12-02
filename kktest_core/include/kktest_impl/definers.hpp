#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_DEFINERS_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_DEFINERS_HPP_

#include <kktest_impl/config.hpp>
#include <kktest_impl/types.hpp>

namespace kktest {

class TestCaseDefiner {
 public:
    TestCaseDefiner(void (*testCase)(), const char* name, const unsigned char* signature) noexcept;
};

void test(const TestConfig& config, Executable func);

void test(String description, Executable func);

void group(const GroupConfig& config, Executable func);

void group(String description, Executable func);

void setUp(Executable func);

void tearDown(Executable func);

namespace detail {

void throwExpectationFailed(const String& message);

}

template<class T, class M, class... Args>
void expect(const T& object, M matcher, const Args...) {
    if (matcher.matches(object)) {
        return;
    }
    detail::throwExpectationFailed("Expectation failed:\n\t"
                                   + matcher.buildMismatchMessage(object));
}

template<class... Args>
void expect(const bool& exprResult, const String& expr = "", const Args...) {
    if (!exprResult) {
        detail::throwExpectationFailed(expr);
    }
}

template<class... Args>
void expect(const bool& exprResult, const char* expr, const Args...) {
    if (!exprResult) {
        detail::throwExpectationFailed(expr);
    }
}

void fail(const String& message = "");

}  // namespace kktest

#endif  // KKTEST_CORE_INCLUDE_KKTEST_IMPL_DEFINERS_HPP_
