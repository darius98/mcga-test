#include "description.hpp"

using namespace std;

namespace matcher {

Description* Description::createForExpectation(
        const char* fileName,
        const int& lineNumber,
        const char* extra) {
    auto description = new Description();
    description->append(string(fileName) + ":" + to_string(lineNumber) + ": ");
    description->append("Expected ");
    if (extra != nullptr) {
        description->append(extra);
    }
    return description;
}

string Description::toString() const {
    return stream.str();
}

}