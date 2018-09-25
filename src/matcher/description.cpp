#include "description.hpp"

using namespace std;

namespace matcher {

Description* Description::createForExpectation() {
    return (new Description())->append("expected ");
}

string Description::toString() const {
    return this->stream.str();
}

}