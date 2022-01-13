#include "core/extension.hpp"

namespace mcga::test {

class FilterExtension : public Extension {
  public:
    void registerCommandLineArgs(cli::Parser* parser) override;

    void init(ExtensionApi* api) override;
};

}  // namespace mcga::test
