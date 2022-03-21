#include "core/extension.hpp"

#include <functional>
#include <string>
#include <vector>

namespace mcga::test {

class FilterExtension {
  public:
    FilterExtension(const std::vector<std::string>& descriptionFilter,
                    const std::vector<std::string>& descriptionExclude,
                    const std::vector<std::string>& locationFilter,
                    const std::vector<std::string>& locationExclude);

    void beforeTestExecution(const Test& test,
                             std::optional<Test::ExecutionInfo>& info);

  private:
    [[nodiscard]] bool shouldSkipTest(const Test& test) const;

    std::vector<std::function<bool(const Test&)>> filters;
};

}  // namespace mcga::test
