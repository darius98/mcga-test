#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_ARGUMENTS_API_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_ARGUMENTS_API_H_

#include <string>
#include <vector>

namespace kktest {

class Flag {
public:
    virtual ~Flag() = default;

    virtual bool get() const = 0;
};

class Argument {
public:
    virtual ~Argument() = default;

    virtual std::string get() const = 0;
};

class ArgumentsApi {
public:
    static ArgumentsApi* create(const std::string& helpPrefix);

    virtual ~ArgumentsApi() = default;

    virtual Argument* addArgument(const std::string& name,
                                  const std::string& helpText,
                                  const std::string& shortName,
                                  const std::string& defaultValue,
                                  const std::string& implicitValue) = 0;

    virtual Flag* addFlag(const std::string& name,
                          const std::string& helpText,
                          const std::string& shortName) = 0;

    virtual std::vector<std::string> interpret(int argc, char** argv) = 0;

    inline Argument* addArgument(const std::string& name,
                                 const std::string& helpText,
                                 const std::string& shortName,
                                 const std::string& defaultValue) {
        return addArgument(name, helpText, shortName, defaultValue, "");
    }

    inline Argument* addArgument(const std::string& name,
                                 const std::string& helpText,
                                 const std::string& shortName) {
        return addArgument(name, helpText, shortName, "");
    }

    inline Argument* addArgument(const std::string& name, const std::string& helpText) {
        return addArgument(name, helpText, "");
    }

    inline Flag* addFlag(const std::string& name, const std::string& helpText) {
        return addFlag(name, helpText, "");
    }
};

}

#endif
