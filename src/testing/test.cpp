#include "test.hpp"

using namespace std;

namespace runtime_testing {

Test::Test(const std::string &description): description(description) {}

Test::~Test() {
    delete this->failure;
}

void Test::generateTestReport(std::ostream &report, std::size_t spaces) {
    string prefix(spaces + 2, ' ');
    report << "{\n";
    report << prefix << R"("description": ")" << this->description << "\",\n";
    report << prefix
           << R"("passed": )"
           << (this->failure == nullptr ? "true" : "false");
    if (this->failure != nullptr) {
        report << ",\n";
        report << prefix
               << R"("failureMessage": ")"
               << this->failure->getMessage()
               << "\"\n";
    } else {
        report << "\n";
    }
    report << string(spaces, ' ') << "}";
}

}
