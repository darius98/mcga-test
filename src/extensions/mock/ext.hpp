#pragma once

#include "core/extension.hpp"

namespace mcga::test::mock {

class MockExtension : public Extension {
  public:
    void init(ExtensionApi* api) override;
};

}  // namespace mcga::test::feedback
