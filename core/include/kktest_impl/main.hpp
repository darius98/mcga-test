#ifndef KKTEST_CORE_KKTEST_IMPL_MAIN_HPP_
#define KKTEST_CORE_KKTEST_IMPL_MAIN_HPP_

#include <vector>

#include <kktest_impl/extension_api.hpp>

namespace kktest {

struct InternalArgs {
    cppli::Flag smoothFlag;
    cppli::NumericArgument<std::size_t> maxParallelTestsArgument;
};

InternalArgs registerInternalFlags(cppli::Parser& parser,
                                   const std::string& versionString);

int main(const std::vector<Extension*>& extensions, InternalArgs internalFlags);

}

#endif
