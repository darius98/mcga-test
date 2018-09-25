#include "test.hpp"

using namespace std;

namespace runtime_testing {

Test::Test(const std::string &description): description(description) {}

Test::~Test() {
    delete this->failure;
}

void Test::report(ostream &report, const string &currentGroupFullName) {
    report << currentGroupFullName
           << " > "
           << this->description
           << ": "
           << (this->failure != nullptr ? "FAILED" : "PASSED")
           << "\n";
    if (this->failure != nullptr) {
        report << "\t" << this->failure->getMessage() << "\n";
    }
}

}
