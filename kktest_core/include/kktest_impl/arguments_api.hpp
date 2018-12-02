#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_ARGUMENTS_API_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_ARGUMENTS_API_HPP_

#include <vector>

#include <kktest_impl/types.hpp>

namespace kktest {

class Flag {
 public:
    virtual ~Flag() = default;

    virtual bool get() const = 0;
};

class Argument {
 public:
    virtual ~Argument() = default;

    virtual String get() const = 0;
};

class ArgumentsApi {
 public:
    static ArgumentsApi* create(const String& helpPrefix);

    virtual ~ArgumentsApi() = default;

    virtual Argument* addArgument(const String& name,
                                  const String& helpText,
                                  const String& shortName,
                                  const String& defaultValue,
                                  const String& implicitValue) = 0;

    virtual Flag* addFlag(const String& name,
                          const String& helpText,
                          const String& shortName) = 0;

    virtual std::vector<String> interpret(int argc, char** argv) = 0;

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

    inline Flag* addFlag(const String& name, const String& helpText) {
        return addFlag(name, helpText, "");
    }
};

}  // namespace kktest

#endif  // KKTEST_CORE_INCLUDE_KKTEST_IMPL_ARGUMENTS_API_HPP_
