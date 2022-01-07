#include <iostream>

#include "mcga/test.hpp"
#include "mcga/test_ext/matchers.hpp"

#include "mcga/test_ext/mock.hpp"

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
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);
        mock::libc.malloc.replace(malloc_replacement);
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isTrue);

        expect(malloc(14), isEqualTo(nullptr));
        expect(arg, isEqualTo(14));
    });

    test("replacement function from previous test is not called", [&] {
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);

        auto ptr = malloc(14);
        expect(ptr, isNotEqualTo(nullptr));
        expect(arg, isZero);
        free(ptr);
    });

    test("replacement function is not called before replace", [&] {
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);

        auto ptr = malloc(14);
        expect(ptr, isNotEqualTo(nullptr));
        expect(arg, isZero);
        free(ptr);

        mock::libc.malloc.replace(malloc_replacement);

        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isTrue);

        expect(malloc(14), isEqualTo(nullptr));
        expect(arg, isEqualTo(14));
    });

    test("replacement function is not called after reset", [&] {
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);
        mock::libc.malloc.replace(malloc_replacement);
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isTrue);

        expect(malloc(14), isEqualTo(nullptr));
        expect(arg, isEqualTo(14));

        mock::libc.malloc.reset();
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);

        arg = 0;

        auto ptr = malloc(14);
        expect(ptr, isNotEqualTo(nullptr));
        expect(arg, isZero);
        free(ptr);
    });

    test("replacement for noreturn function", [&] {
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);
        mock::libc.abort.replace([&] {
            throw std::runtime_error("Something");
        });
        expect(mock::libc.abort.is_replaced(), isTrue);
        expect(mock::libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Something"));
        }
    });

    test("replacement for noreturn function that returns", [&] {
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);
        mock::libc.abort.replace([&] {});
        expect(mock::libc.abort.is_replaced(), isTrue);
        expect(mock::libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Replacement for function marked [[noreturn]] returned."));
        }
    });

    test("replacement for function with variadic arguments", [&] {
        mock::libc.printf.replace([&](const char* fmt, va_list va) {
            expect(fmt, isEqualTo("%d %d"));
            expect(va_arg(va, int), 3);
            expect(va_arg(va, int), 4);
            return 777;
        });

        expect(printf("%d %d", 3, 4), isEqualTo(777));
    });

    test("multiple mocks in same test", [&] {
        mock::libc.malloc.replace(malloc_replacement);
        mock::libc.abort.replace([&] {
            throw std::runtime_error("Something");
        });
        mock::libc.printf.replace([&](const char* fmt, va_list va) {
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
        expect(mock::libc.abort.is_replaced(), isFalse);
        expect(mock::libc.malloc.is_replaced(), isFalse);
        mock::libc.abort.replace([&] {
            throw std::runtime_error("Something");
        });
        expect(mock::libc.abort.is_replaced(), isTrue);
        expect(mock::libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Something"));
        }

        mock::libc.abort.replace([&] {
            throw std::runtime_error("Something else");
        });
        expect(mock::libc.abort.is_replaced(), isTrue);
        expect(mock::libc.malloc.is_replaced(), isFalse);

        try {
            std::abort();
        } catch (const std::runtime_error& err) {
            expect(err.what(), isEqualTo("Something else"));
        }
    });
}
