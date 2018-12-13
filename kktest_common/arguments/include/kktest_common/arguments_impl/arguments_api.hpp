#ifndef KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENTS_API_HPP_
#define KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENTS_API_HPP_

#include <vector>

#include <kktest_common/arguments_impl/argument.hpp>

namespace kktest {
namespace arguments {

class ArgumentsApi {
 public:
    static ArgumentsApi* create(const String& helpPrefix);

    virtual ~ArgumentsApi() = default;

    virtual Argument* addArgument(const ArgumentBuilder& builder) = 0;

    virtual IntArgument* addIntArgument(const IntArgumentBuilder& builder) = 0;

    virtual Flag* addFlag(const FlagBuilder& builder) = 0;

    virtual std::vector<String> interpret(int argc, char** argv) = 0;

    virtual void addHelpFlag() = 0;

    virtual void checkHelpFlag() = 0;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENTS_API_HPP_
