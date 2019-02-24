#ifndef KKTEST_COMMON_INTERPROC_SRC_ERRORS_HPP_
#define KKTEST_COMMON_INTERPROC_SRC_ERRORS_HPP_

#include <stdexcept>

namespace kktest {
namespace interproc {

struct InterprocError: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}
}

#endif
