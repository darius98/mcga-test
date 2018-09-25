#include "description.hpp"

using namespace std;

namespace matcher {

Description* Description::createForExpectation(
        const char* fileName,
        const int& lineNumber) {
    auto description = new Description();
    description->append(string(fileName) + ":" + to_string(lineNumber) + ": ");
    description->append("expected ");
    return description;
}

string Description::toString() const {
    return this->stream.str();
}

}