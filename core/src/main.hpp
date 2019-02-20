#ifndef KKTEST_CORE_SRC_MAIN_HPP_
#define KKTEST_CORE_SRC_MAIN_HPP_

#include <vector>

#include "core/src/extension_api.hpp"

namespace kktest {

int main(int argc, char** argv, const std::vector<Extension*>& extensions);

}

#endif
