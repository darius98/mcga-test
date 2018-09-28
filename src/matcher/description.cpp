#include "description.hpp"

using namespace std;

namespace matcher {

Description* Description::createForExpectation(
        const char* fileName,
        const int& lineNumber,
        const string& extra) {
    auto description = new Description();
    description->append(string(fileName) + ":" + to_string(lineNumber) + ": ");
    description->append(extra);
    if (extra.empty()) {
        description->append("Expected ");
    }
    return description;
}

string Description::toString() const {
    return stream.str();
}

}