#include "testing/test.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

Test::Test(string description, string file, int line):
        description(move(description)), file(move(file)), line(line) {}

Test::~Test() {
    delete failure;
}

JSON Test::generateReport() const {
    JSON report;
    report["description"] = description;
    report["passed"] = (failure == nullptr);
    if (!file.empty()) {
        report["file"] = file;
        report["line"] = line;
    }
    if (failure != nullptr) {
        report["failureMessage"] = failure->getMessage();
    }
    return report;
}

}
