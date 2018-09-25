#include "test.hpp"

using namespace std;

namespace runtime_testing {

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
