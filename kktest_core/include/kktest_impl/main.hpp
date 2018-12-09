#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_MAIN_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_MAIN_HPP_

#include <vector>

#include <kktest_impl/extension_api.hpp>

namespace kktest {

struct InternalArgs {
    arguments::Flag* versionFlag = nullptr;
    arguments::Flag* getSignatureFlag = nullptr;
    arguments::Flag* boxedFlag = nullptr;
    arguments::IntArgument* maxParallelTestsArgument = nullptr;
};

InternalArgs registerInternalFlags(arguments::ArgumentsApi* argumentsApi);

int main(const std::vector<Extension*>& extensions, InternalArgs internalFlags);

}  // namespace kktest

#endif  // KKTEST_CORE_INCLUDE_KKTEST_IMPL_MAIN_HPP_
