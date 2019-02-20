#ifndef KKTEST_COMMON_INTERPROC_SRC_ERRORS_HPP_
#define KKTEST_COMMON_INTERPROC_SRC_ERRORS_HPP_

#include <stdexcept>

namespace kktest {
namespace interproc {

class InterprocError: public std::runtime_error {
 public:
    explicit InterprocError(const std::string& what);
};

}
}

#endif
