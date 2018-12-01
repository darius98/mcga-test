#ifndef KKTEST_CORE_ARGUMENTS_API_IMPL_H_
#define KKTEST_CORE_ARGUMENTS_API_IMPL_H_

#include <map>
#include <set>

#include <kktest_impl/arguments_api.hpp>

namespace kktest {

class ArgumentSpec {
public:
    ArgumentSpec(const std::string& defaultValue, const std::string& _implicitValue);

    ~ArgumentSpec();

    Argument* getArgument();

    void setImplicit();

    void setValue(const std::string& value);

private:
    std::string implicitValue;
    Argument* argument;
};

class FlagSpec {
public:
    FlagSpec();

    ~FlagSpec();

    Flag* getFlag();

    void setValue(bool value);

private:
    Flag* flag;
};

class ArgumentsApiImpl: public ArgumentsApi {
public:
    explicit ArgumentsApiImpl(std::string helpPrefix);

    ~ArgumentsApiImpl() override;

    Argument* addArgument(const std::string& name,
                          const std::string& helpText,
                          const std::string& shortName,
                          const std::string& defaultValue,
                          const std::string& implicitValue) override;

    Flag* addFlag(const std::string& name,
                  const std::string& helpText,
                  const std::string& shortName) override;

    std::vector<std::string> interpret(int argc, char** argv) override;

    void addHelpFlag();

    std::string getHelpSection(const std::string& name,
                               const std::string& helpText,
                               const std::string& shortName,
                               const std::string& defaultValue,
                               const std::string& implicitValue);

private:
    void applyValue(const std::string& commandLineString, const std::string& value);

    void applyImplicit(const std::string& commandLineString);

    Flag* helpFlag = nullptr;

    std::vector<ArgumentSpec*> argumentsSpec;
    std::vector<FlagSpec*> flagsSpec;

    std::string help;
    std::set<std::string> reservedNames;
    std::map<std::string, ArgumentSpec*> argumentSpecsByCommandLineString;
    std::map<std::string, FlagSpec*> flagSpecsByCommandLineString;
};

}

#endif
