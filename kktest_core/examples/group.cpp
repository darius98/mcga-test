#include <vector>

#include <kktest.hpp>

using kktest::expect;
using kktest::fail;
using kktest::group;
using kktest::setUp;
using kktest::test;

void kkTestCase() {
    std::vector<int> v;

    setUp([&] {
        v = std::vector<int>{};  // always start with a clean vector
    });

    test("Vector is initially empty", [&] {
        expect(v.empty());
    });

    test("After one push_back, vector is not empty anymore", [&] {
        v.push_back(3);
        expect(!v.empty());
    });

    group("After inserting 5 elements", [&] {
        setUp([&] {
            v.insert(v.end(), {1, 2, 3, 4, 5});
        });

        test("Size of the vector is 5", [&] {
            expect(v.size() == 5);
        });

        test("v.at(4) does not throw", [&] {
            try {
                v.at(4);
            } catch(...) {
                fail("It did throw.");
            }
        });
    });

    test("v.at(4) throws on empty vector", [&] {
        try {
            v.at(4);
            fail("Did not throw.");
        } catch(...) { /* All ok; */ }
    });
}
