#include "core/extension.hpp"

#include <regex>

#include <mcga/cli.hpp>

namespace mcga::test {

class FilterExtension {
  public:
    void registerCommandLineArgs(cli::Parser* parser);

    void init(ExtensionApi* api);

    void destroy() {}

  private:
    [[nodiscard]] bool shouldSkipTest(const Test& test) const;

    std::vector<std::function<bool(const Test&)>> filters;

    cli::ListArgument<> descriptionFilterArgument;
    cli::ListArgument<> descriptionExcludeArgument;
    cli::ListArgument<> locationFilterArgument;
    cli::ListArgument<> locationExcludeArgument;
};

}  // namespace mcga::test
