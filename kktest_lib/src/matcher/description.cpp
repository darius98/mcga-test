#include "description.hpp"

using namespace std;


namespace kktest {

Description* Description::createForExpect(
        const string& file,
        int line,
        const string& extra) {
    auto description = new Description();
    (*description) << file << ":" << line << ": " << extra;
    return description;
}

string Description::toString() const {
    return stream.str();
}

}
