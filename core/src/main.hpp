#ifndef KKTEST_CORE_SRC_MAIN_HPP_
#define KKTEST_CORE_SRC_MAIN_HPP_

#include <vector>

#include "core/src/extension_api.hpp"

namespace kktest {

/** Entry-point of the core library.
 *
 * This function has the responsibility to instantiate a testing Driver,
 * initialize the extensions, parse command line arguments and actually run the
 * test cases provided.
 *
 * This should only be called once during the run() public entry point. */
int main(int argc, char** argv,
         std::vector<TestCase>&& tests,
         const std::vector<Extension*>& extensions);

}

#endif
