#include "test.hpp"

using namespace std;

namespace runtime_testing {

Test::Test(string description, string file, int line):
        description(move(description)), file(move(file)), line(line) {}

Test::~Test() {
    delete failure;
}

void Test::generateTestReport(ostream &report, size_t spaces) {
    string prefix(spaces + 2, ' ');
    report << "{\n";
    report << prefix << R"("description": ")" << description << "\",\n";
    if (!file.empty()) {
        report << prefix << R"("file": ")" << file << "\",\n";
        report << prefix << "\"line\": " << line << ",\n";
    }
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
