#include <kktest_common/arguments_impl/arguments_api.hpp>
#include "arguments_api_impl.hpp"

namespace kktest {
namespace arguments {

ArgumentsApi* ArgumentsApi::create(const String& helpPrefix) {
    auto api = new ArgumentsApiImpl(helpPrefix);
    api->addHelpFlag();
    return api;
}

}  // namespace arguments
}  // namespace kktest
