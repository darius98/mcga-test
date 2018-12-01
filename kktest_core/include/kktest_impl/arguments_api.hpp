#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_ARGUMENTS_API_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_ARGUMENTS_API_H_

#include <string>
#include <vector>

namespace kktest {

class Flag {
public:
    Flag();

    bool get() const;

private:
    void setValue(bool _value);

    bool value;

friend class FlagSpec;
};

class Argument {
public:
    explicit Argument(std::string _value);

    std::string get() const;

private:
    void setValue(const std::string& _value);

    std::string value;

friend class ArgumentSpec;
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
