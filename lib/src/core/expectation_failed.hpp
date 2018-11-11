#ifndef KKTEST_CORE_EXPECTATION_FAILED_H_
#define KKTEST_CORE_EXPECTATION_FAILED_H_

#include <stdexcept>
#include <string>


namespace kktest {

class ExpectationFailed: public std::runtime_error {
public:
    explicit ExpectationFailed(const std::string& str);
};

}

#endif
