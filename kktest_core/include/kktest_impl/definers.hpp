#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_DEFINERS_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_DEFINERS_H_

#include <string>

#include <kktest_impl/config.hpp>
#include <kktest_impl/executable.hpp>

namespace kktest {

class TestCaseDefiner {
public:
    TestCaseDefiner(void (*testCase)(), const unsigned char* signature);
};

void test(const TestConfig& config, Executable func);

void test(std::string description, Executable func);

void group(const GroupConfig& config, Executable func);

void group(std::string description, Executable func);

void setUp(Executable func);

void tearDown(Executable func);

namespace detail {

void throwExpectationFailed(const std::string& message);

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
void expect(const bool& exprResult, const std::string& expr="", const Args...) {
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

void fail(const std::string& message="");

}

#endif
