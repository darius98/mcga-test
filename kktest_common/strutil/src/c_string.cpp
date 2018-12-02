#include <cstring>
#include <cstdlib>

#include <kktest_common/strutil_impl/c_string.hpp>

using std::malloc;
using std::memcpy;
using std::size_t;
using std::string;

namespace kktest {
namespace strutil {

char* copyAsCString(const char* source) {
    size_t size = (strlen(source) + 1) * sizeof(char);
    auto destination = static_cast<char*>(malloc(size));
    memcpy(destination, source, size);
    return destination;
}

char* copyAsCString(const string& source) {
    return copyAsCString(source.c_str());
}

}  // namespace strutil
}  // namespace kktest
