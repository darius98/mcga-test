#ifndef KKTEST_COMMON_ARGUMENTS_SRC_ARGUMENTS_API_IMPL_HPP_
#define KKTEST_COMMON_ARGUMENTS_SRC_ARGUMENTS_API_IMPL_HPP_

#include <map>
#include <set>
#include <vector>

#include <kktest_common/arguments_impl/arguments_api.hpp>
#include "command_line_spec.hpp"

namespace kktest {
namespace arguments {

class ArgumentsApiImpl: public ArgumentsApi {
 public:
    explicit ArgumentsApiImpl(String helpPrefix);

    ~ArgumentsApiImpl() override;

    Argument* addArgument(const String& name,
                          const String& helpText,
                          const String& shortName,
                          const String& defaultValue,
                          const String& implicitValue) override;

    IntArgument* addIntArgument(const String& name,
                                const String& helpText,
                                const String& shortName,
                                int defaultValue,
                                int implicitValue) override;

    Flag* addFlag(const String& name,
                  const String& helpText,
                  const String& shortName) override;

    std::vector<String> interpret(int argc, char** argv) override;

    void addHelpFlag() override;

    void checkHelpFlag() override;

 private:
    void addSpec(CommandLineSpec* spec,
                 const String& name,
                 const String& helpText,
                 const String& shortName,
                 const String& defaultValue,
                 const String& implicitValue);

    void checkNameAvailability(const String& name,
                               const String& shortName) const;

    void applyValue(const String& commandLineString, const String& value);

    void applyImplicit(const String& commandLineString);

    Flag* helpFlag = nullptr;

    std::vector<CommandLineSpec*> commandLineSpecs;
    std::map<String, CommandLineSpec*> specsByCommandLineString;

    String help;
    std::set<String> reservedNames;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_SRC_ARGUMENTS_API_IMPL_HPP_
