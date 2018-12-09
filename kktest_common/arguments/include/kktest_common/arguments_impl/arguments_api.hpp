#ifndef KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENTS_API_HPP_
#define KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENTS_API_HPP_

#include <vector>

#include <kktest_common/arguments_impl/argument.hpp>
#include <kktest_common/arguments_impl/flag.hpp>
#include <kktest_common/arguments_impl/int_argument.hpp>

namespace kktest {
namespace arguments {

class ArgumentsApi {
 public:
    static ArgumentsApi* create(const String& helpPrefix);

    virtual ~ArgumentsApi() = default;

    virtual Argument* addArgument(const String& name,
                                  const String& helpText,
                                  const String& shortName,
                                  const String& defaultValue,
                                  const String& implicitValue) = 0;

    virtual IntArgument* addIntArgument(const String& name,
                                        const String& helpText,
                                        const String& shortName,
                                        int defaultValue,
                                        int implicitValue) = 0;

    virtual Flag* addFlag(const String& name,
                          const String& helpText,
                          const String& shortName) = 0;

    virtual std::vector<String> interpret(int argc, char** argv) = 0;

    virtual void addHelpFlag() = 0;

    virtual void checkHelpFlag() = 0;

    inline Argument* addArgument(const String& name,
                                 const String& helpText,
                                 const String& shortName,
                                 const String& defaultValue) {
        return addArgument(name, helpText, shortName, defaultValue, "");
    }

    inline Argument* addArgument(const String& name,
                                 const String& helpText,
                                 const String& shortName) {
        return addArgument(name, helpText, shortName, "");
    }

    inline Argument* addArgument(const String& name, const String& helpText) {
        return addArgument(name, helpText, "");
    }

    inline IntArgument* addIntArgument(const String& name,
                                       const String& helpText,
                                       const String& shortName,
                                       int defaultValue) {
        return addIntArgument(name, helpText, shortName, defaultValue, 0);
    }

    inline IntArgument* addIntArgument(const String& name,
                                       const String& helpText,
                                       const String& shortName) {
        return addIntArgument(name, helpText, shortName, 0);
    }

    inline IntArgument* addIntArgument(const String& name,
                                       const String& helpText) {
        return addIntArgument(name, helpText, "");
    }

    inline Flag* addFlag(const String& name, const String& helpText) {
        return addFlag(name, helpText, "");
    }
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENTS_API_HPP_
