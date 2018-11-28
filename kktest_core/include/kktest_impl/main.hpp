#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_MAIN_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_MAIN_H_

#include <vector>

#include <kktest_impl/plugin_api.hpp>

namespace kktest {

int main(const std::vector<Plugin*>& plugins);

}

#endif
