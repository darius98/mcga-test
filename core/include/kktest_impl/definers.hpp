#ifndef KKTEST_CORE_KKTEST_IMPL_DEFINERS_HPP_
#define KKTEST_CORE_KKTEST_IMPL_DEFINERS_HPP_

#include <kktest_common/string.hpp>
#include <kktest_impl/config.hpp>
#include <kktest_impl/executable.hpp>

namespace kktest {

class TestCaseDefiner {
 public:
    TestCaseDefiner(void (*testCase)(),
                    const char* name,
                    const unsigned char* signature) noexcept;
};

void test(const TestConfig& config, Executable func);

void test(String description, Executable func);

void group(const GroupConfig& config, Executable func);

void group(String description, Executable func);

void setUp(Executable func);

void tearDown(Executable func);

void fail(const String& message = "");

void expect(const bool& exprResult, const String& expr = "");

void expect(const bool& exprResult, const char* expr);

}

#endif
