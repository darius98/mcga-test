#include "common/utils/src/string.hpp"

#include <cstring>

using namespace std;

namespace kktest {
namespace utils {

char* copyAsCString(const string& s) {
    auto size = (s.length() + 1) * sizeof(char);
    auto destination = malloc(size);
    memcpy(destination, s.c_str(), size);
    return static_cast<char*>(destination);
}

}
}
