#include <iostream>

#include "mcga/test.hpp"
#include "mcga/test_ext/matchers.hpp"

#include "mcga/test_ext/mock.hpp"
#include "mcga/test_ext/mock_helpers.hpp"

using mcga::test::FunctionMock;

struct LibCMocks {
    FunctionMock<void()> abort{"abort"};
    FunctionMock<void*(size_t)> malloc{"malloc"};
    FunctionMock<int(const char*, va_list)> vprintf{"vprintf"};
    FunctionMock<int(const char*, ...)> printf{&vprintf};
};

static constinit LibCMocks libc;

DECLARE_NORETURN_FUNCTION(libc, abort, 0)
DECLARE_FUNCTION(libc, malloc, 1)
DECLARE_FUNCTION(libc, vprintf, 2)
DECLARE_VA_FUNCTION(libc, printf, 1)

using namespace mcga::test;
using namespace mcga::matchers;

TEST_CASE("mock extension") {
    size_t arg = 0;

    const auto malloc_replacement = [&](size_t n) {
        arg = n;
        return nullptr;
    };

    tearDown([&] {
        arg = 0;
    });

    test("replacement function is called", [&] {
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);
        libc.malloc.replace(malloc_replacement);
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isTrue);

        expect(malloc(14), isEqualTo(nullptr));
        expect(arg, isEqualTo(14));
    });

    test("replacement function from previous test is not called", [&] {
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);

        auto ptr = malloc(14);
        expect(ptr, isNotEqualTo(nullptr));
        expect(arg, isZero);
        free(ptr);
    });

    test("replacement function is not called before replace", [&] {
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);

        auto ptr = malloc(14);
        expect(ptr, isNotEqualTo(nullptr));
        expect(arg, isZero);
        free(ptr);

        libc.malloc.replace(malloc_replacement);

        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isTrue);

        expect(malloc(14), isEqualTo(nullptr));
        expect(arg, isEqualTo(14));
    });

    test("replacement function is not called after reset", [&] {
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);
        libc.malloc.replace(malloc_replacement);
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isTrue);

        expect(malloc(14), isEqualTo(nullptr));
        expect(arg, isEqualTo(14));

        libc.malloc.reset();
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);

        arg = 0;

        auto ptr = malloc(14);
        expect(ptr, isNotEqualTo(nullptr));
        expect(arg, isZero);
        free(ptr);
    });

    test("replacement for noreturn function", [&] {
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);
        libc.abort.replace([&] {
            throw std::runtime_error("Something");
        });
        expect(libc.abort.is_replaced(), isTrue);
        expect(libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Something"));
        }
    });

    test("replacement for noreturn function that returns", [&] {
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);
        libc.abort.replace([&] {});
        expect(libc.abort.is_replaced(), isTrue);
        expect(libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(),
                   isEqualTo(
                     "Replacement for function marked [[noreturn]] returned."));
        }
    });

    test("replacement for function with variadic arguments", [&] {
        libc.printf.replace([&](const char* fmt, va_list va) {
            expect(fmt, isEqualTo("%d %d"));
            expect(va_arg(va, int), 3);
            expect(va_arg(va, int), 4);
            return 777;
        });

        expect(printf("%d %d", 3, 4), isEqualTo(777));
    });

    test("multiple mocks in same test", [&] {
        libc.malloc.replace(malloc_replacement);
        libc.abort.replace([&] {
            throw std::runtime_error("Something");
        });
        libc.printf.replace([&](const char* fmt, va_list va) {
            expect(fmt, isEqualTo("%d %d"));
            expect(va_arg(va, int), 3);
            expect(va_arg(va, int), 4);
            return 777;
        });

        expect(malloc(14), isEqualTo(nullptr));
        expect(arg, isEqualTo(14));

        expect(printf("%d %d", 3, 4), isEqualTo(777));

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Something"));
        }
    });

    test("mocks from previous test are not still active", [&] {
        auto ptr = malloc(14);
        expect(ptr, isNotEqualTo(nullptr));
        expect(arg, isZero);
        free(ptr);

        expect(printf("%d %d", 3, 4), isEqualTo(3));
    });

    test("mock replaced multiple times in same test", [&] {
        expect(libc.abort.is_replaced(), isFalse);
        expect(libc.malloc.is_replaced(), isFalse);
        libc.abort.replace([&] {
            throw std::runtime_error("Something");
        });
        expect(libc.abort.is_replaced(), isTrue);
        expect(libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Something"));
        }

        libc.abort.replace([&] {
            throw std::runtime_error("Something else");
        });
        expect(libc.abort.is_replaced(), isTrue);
        expect(libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Something else"));
        }
    });
}
