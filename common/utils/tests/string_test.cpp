#include <cstring>

#include <kktest.hpp>
#include <kktest_ext/matchers.hpp>

#include "common/utils/src/string.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace kktest::utils;
using namespace std;

void kkTestCase(UtilsString) {
    test("copyAsCStr()", [&] {
        string s = "cpp_string";
        char* result = copyAsCString(s);
        expect(strcmp(result, s.c_str()), isZero);
        free(result);
    });
}
