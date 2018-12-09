#include <cstring>

#include <kktest.hpp>

#include <kktest_common/strutil_impl/c_string.hpp>

using kktest::expect;
using kktest::group;
using kktest::tearDown;
using kktest::test;
using kktest::strutil::copyAsCString;
using std::string;

void kkTestCase(StrUtilCString) {
    char* result = nullptr;

    tearDown([&] {
        free(result);
    });

    test("copyAsCString(string literal)", [&]() {
        result = copyAsCString("string_literal");
        expect(strcmp(result, "string_literal") == 0);
    });

    test("copyAsCString(const char*)", [&]() {
        const char* ptr = "const_pointer";
        result = copyAsCString(ptr);
        expect(strcmp(result, ptr) == 0);
    });

    test("copyAsCString(char*)", [&]() {
        const char* value = "raw_pointer";
        size_t len = strlen(value);
        auto ptr = static_cast<char*>(malloc((len + 1) * sizeof(char)));
        memcpy(ptr, value, (len + 1) * sizeof(char));
        result = copyAsCString(ptr);
        expect(strcmp(result, ptr) == 0);
        free(ptr);
    });

    test("copyAsCString(std::string)", [&]() {
        string s = "cpp_string";
        result = copyAsCString(s);
        expect(strcmp(result, s.c_str()) == 0);
    });
}
