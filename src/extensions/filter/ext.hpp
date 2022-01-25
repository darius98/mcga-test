#include "core/extension.hpp"

#include <regex>

#include <mcga/cli.hpp>

namespace mcga::test {

class FilterExtension {
  public:
    void registerCommandLineArgs(cli::Parser* parser);

    void init();

    void beforeTestExecution(const Test& test,
                             std::optional<Test::ExecutionInfo>& info);

  private:
    [[nodiscard]] bool shouldSkipTest(const Test& test) const;

    std::vector<std::function<bool(const Test&)>> filters;

    cli::ListArgument<> descriptionFilterArgument;
    cli::ListArgument<> descriptionExcludeArgument;
    cli::ListArgument<> locationFilterArgument;
    cli::ListArgument<> locationExcludeArgument;
};

}  // namespace mcga::test
