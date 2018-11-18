#ifndef KKTEST_CORE_ERRORS_H_
#define KKTEST_CORE_ERRORS_H_

#include <stdexcept>
#include <string>


namespace kktest {

class ExpectationFailed: public std::runtime_error {
public:
    explicit ExpectationFailed(const std::string& str);
};

}

#endif
