#include "test.hpp"

using namespace std;

namespace runtime_testing {

Test::Test(const std::string &description): description(description) {}

Test::~Test() {
    delete failure;
}

void Test::generateTestReport(std::ostream &report, std::size_t spaces) {
    string prefix(spaces + 2, ' ');
    report << "{\n";
    report << prefix << R"("description": ")" << description << "\",\n";
    report << prefix
           << R"("passed": )"
           << (failure == nullptr ? "true" : "false");
    if (failure != nullptr) {
        report << ",\n";
        report << prefix
               << R"("failureMessage": ")"
               << failure->getMessage()
               << "\"\n";
    } else {
        report << "\n";
    }
    report << string(spaces, ' ') << "}";
}

}
