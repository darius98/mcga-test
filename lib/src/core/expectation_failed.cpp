#include "expectation_failed.hpp"

using namespace std;


namespace kktest {

ExpectationFailed::ExpectationFailed(const string& str): runtime_error(str) {}

ExpectationFailed::ExpectationFailed(const ExpectationFailed& other) noexcept:
        ExpectationFailed(other.getMessage()) {}

ExpectationFailed::ExpectationFailed(ExpectationFailed&& other) noexcept:
        ExpectationFailed(other.getMessage()) {}

string ExpectationFailed::getMessage() const {
    return what();
}

}
