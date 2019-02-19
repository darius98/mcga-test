#ifndef KKTEST_CORE_SRC_MAIN_HPP_
#define KKTEST_CORE_SRC_MAIN_HPP_

#include <vector>

#include "core/src/extension_api.hpp"

namespace kktest {

struct InternalArgs {
    cppli::Flag smoothFlag;
    cppli::NumericArgument<std::size_t> maxParallelTestsArgument;
};

InternalArgs registerInternalFlags(cppli::Parser& parser, std::string version);

int main(const std::vector<Extension*>& extensions, InternalArgs internalFlags);

}

#endif
