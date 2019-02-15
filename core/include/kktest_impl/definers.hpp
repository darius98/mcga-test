#ifndef KKTEST_CORE_KKTEST_IMPL_DEFINERS_HPP_
#define KKTEST_CORE_KKTEST_IMPL_DEFINERS_HPP_

#include <kktest_common/string.hpp>
#include <kktest_impl/config.hpp>
#include <kktest_impl/executable.hpp>
#include <kktest_impl/export.hpp>

namespace kktest {

class KKTEST_API TestCaseDefiner {
 public:
    TestCaseDefiner(void (*testCase)(),
                    const char* name,
                    const unsigned char* signature) noexcept;
};

KKTEST_API void test(const TestConfig& config, Executable func);

KKTEST_API void test(String description, Executable func);

KKTEST_API void group(const GroupConfig& config, Executable func);

KKTEST_API void group(String description, Executable func);

KKTEST_API void setUp(Executable func);

KKTEST_API void tearDown(Executable func);

KKTEST_API void fail(const String& message = "");

KKTEST_API void expect(bool exprResult, const String& failMessage = "");

KKTEST_API void expect(bool exprResult, const char* failMessage);

KKTEST_API void expect(bool exprResult, const std::string& failMessage);

}

#endif
